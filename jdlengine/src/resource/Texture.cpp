#include "resource/Texture.hpp"

#include "utils/Logger.hpp"

#include "vulkan/VulkanCommandBuffer.hpp"
#include "vulkan/VulkanContext.hpp"
#include "vulkan/VulkanUtils.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>


namespace jdl
{
namespace resource
{

Texture::Texture(const std::string& name) : Resource(name)
{
    m_device = vk::VulkanContext::GetDevice().get();
}

void Texture::create(const std::string& path)
{
    // Open and read the texture image
    stbi_uc* texels = stbi_load(path.c_str(), &m_width, &m_height, &m_nbChannels, STBI_rgb_alpha);
    if (!texels) {
        JDL_FATAL("Failed to read texture image {}", path);
    }
    doCreate(texels);

    stbi_image_free(texels);
}

void Texture::create(int width, int height, const unsigned char* texels)
{
    m_width = width;
    m_height = height;

    doCreate(texels);
}

void Texture::doCreate(const unsigned char* texels)
{
    VkDeviceSize size = 4 * m_width * m_height;

    // Create and fill the staging resource (buffer)
    vk::VulkanBufferWrapper stagingBuffer(
        size,
        vk::BufferUsage::eBufferTransferSrc,
        vk::MemoryProperty::eHostCoherent | vk::MemoryProperty::eHostVisible
    );
    stagingBuffer.setData(texels);

    // Create the device image with the associated device memory.
    createImage();
    // Copy the staging buffer content to the image
    copyBufferToImage(stagingBuffer);
    // Create the sampler
    createSampler();
}

void Texture::createImage()
{
    m_image = std::make_unique<vk::VulkanImage>(
        static_cast<uint32_t>(m_width),
        static_cast<uint32_t>(m_height),
        vk::ImageUsage::eImageTransferDst | vk::ImageUsage::eSampled
    );
}

void Texture::copyBufferToImage(const vk::VulkanBufferWrapper& buffer)
{
    VkCommandPool commandPool = vk::VulkanContext::GetDevice().getCommandPool();
    VkQueue queue = vk::VulkanContext::GetDevice().getGraphicsQueue();

    vk::VulkanCommandBuffer commandBuffer;
    commandBuffer.allocate(commandPool);
    commandBuffer.begin();
    {
        VkCommandBuffer bufferHandle = commandBuffer.get();

        vk::VulkanUtils::TransitionImageLayout(
            bufferHandle,
            m_image->get(),
            m_image->getFormat(),
            VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
        );

        VkBufferImageCopy region {};
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;
        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;
        region.imageOffset = {0, 0, 0};
        region.imageExtent = {static_cast<uint32_t>(m_width), static_cast<uint32_t>(m_height), 1};

        vkCmdCopyBufferToImage(
            bufferHandle,
            buffer.get(),
            m_image->get(),
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            1, &region
        );

        vk::VulkanUtils::TransitionImageLayout(
            bufferHandle,
            m_image->get(),
            m_image->getFormat(),
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        );
    }
    commandBuffer.end();
    commandBuffer.submit(queue, {}, {}, {}, VK_NULL_HANDLE, true);
    commandBuffer.destroy();
}

void Texture::createSampler()
{
    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = static_cast<VkFilter>(m_filtering[FilteringParameter::eMagFilter]);
    samplerInfo.minFilter = static_cast<VkFilter>(m_filtering[FilteringParameter::eMinFilter]);
    samplerInfo.addressModeU = static_cast<VkSamplerAddressMode>(m_wrapping[WrappingParameter::eWrapU]);
    samplerInfo.addressModeV = static_cast<VkSamplerAddressMode>(m_wrapping[WrappingParameter::eWrapV]);
    samplerInfo.addressModeW = static_cast<VkSamplerAddressMode>(m_wrapping[WrappingParameter::eWrapW]);
    samplerInfo.anisotropyEnable = VK_FALSE;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = 0.0f;

    VK_CALL(vkCreateSampler(m_device, &samplerInfo, nullptr, &m_sampler));
}

void Texture::clearResource()
{
    if (m_image)
    {
        vkDestroySampler(m_device, m_sampler, nullptr);
        m_image.reset();
    }
}

} // namespace resource
} // namespace jdl
