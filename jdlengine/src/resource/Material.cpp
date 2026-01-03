#include "resource/Material.hpp"
#include "resource/Texture.hpp"

#include "vulkan/VulkanContext.hpp"
#include "vulkan/VulkanPipeline.hpp"


namespace jdl
{
namespace resource
{

Material::Material(const std::string& name) : Resource(name)
{
    m_device = vk::VulkanContext::GetDevice().get();
    m_materialBuffer = std::make_unique<vk::VulkanBufferWrapper>(
        sizeof(MaterialUniformBuffer),
        vk::BufferUsage::eUniformBuffer,
        vk::MemoryProperty::eHostCoherent | vk::MemoryProperty::eHostVisible
    );
    m_descriptorSet = vk::VulkanContext::GetPipeline().allocateDescriptorSet(vk::DescriptorLayoutPreset::eMaterial);

    // The buffer can already be bound to the descriptor set.
    VkDescriptorBufferInfo bufferInfo {};
    bufferInfo.buffer = m_materialBuffer->get();
    bufferInfo.offset = 0;
    bufferInfo.range = VK_WHOLE_SIZE;

    VkWriteDescriptorSet descriptorWrite {};
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.dstSet = m_descriptorSet;
    descriptorWrite.dstBinding = 0;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrite.descriptorCount = 1;
    descriptorWrite.pBufferInfo = &bufferInfo;

    vkUpdateDescriptorSets(m_device, 1, &descriptorWrite, 0, nullptr);
}

void Material::setBaseColorFactor(const glm::vec4& factor)
{
    m_baseColorFactor = factor;
    m_materialBufferDirty = true;
}

void Material::setBaseColorTexture(Texture* texture)
{
    m_baseColorTexture = texture;
    m_texturesDirty = true;
}

void Material::bind(core::RenderContext& context)
{
    if (m_texturesDirty)
    {
        Texture* baseColorTexture = m_baseColorTexture;
        if (!baseColorTexture) {
            baseColorTexture = resource::ResourceManager::GetAs<resource::Texture>("__DEFAULT_TEXTURE__");
        }

        std::vector<VkDescriptorImageInfo> imageInfos(1);
        // Base Color
        imageInfos[0].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfos[0].imageView = baseColorTexture->getImageView();
        imageInfos[0].sampler = baseColorTexture->getSampler();

        std::vector<VkWriteDescriptorSet> descriptorWrites(imageInfos.size());
        for (auto i = 0; i < descriptorWrites.size(); ++i)
        {
            descriptorWrites[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[i].dstSet = m_descriptorSet;
            descriptorWrites[i].dstBinding = 1 + i;
            descriptorWrites[i].dstArrayElement = 0;
            descriptorWrites[i].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            descriptorWrites[i].descriptorCount = 1;
            descriptorWrites[i].pImageInfo = &imageInfos[i];
        }

        vkUpdateDescriptorSets(
            m_device,
            static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(),
            0, nullptr
        );

        m_texturesDirty = false;
    }
    if (m_materialBufferDirty)
    {
        MaterialUniformBuffer buffer;
        buffer.baseColorFactor = m_baseColorFactor;

        m_materialBuffer->setData(&buffer);
        m_materialBufferDirty = false;
    }

    vkCmdBindDescriptorSets(
        context.commandBuffer,
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        context.pipelineLayout,
        1, 1, &m_descriptorSet,
        0, nullptr
    );
}

void Material::clearResource()
{
    m_materialBuffer.reset();
}

} // namespace resource
} // namespace jdl
