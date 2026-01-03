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
        vk::BufferUsage::eTransferSrc,
        vk::MemoryProperty::eHostCoherent | vk::MemoryProperty::eHostVisible
    );
    stagingBuffer.setData(texels);

    // Create the device image with the associated device memory.
    createImage();
    // Copy the staging buffer content to the image
    copyBufferToImage(stagingBuffer);

    // Create the image view
    m_imageView = vk::VulkanUtils::CreateImageView(
        m_device,
        m_image,
        VK_FORMAT_R8G8B8A8_SRGB,
        VK_IMAGE_ASPECT_COLOR_BIT
    );

    // Create the sampler
    createSampler();
}

void Texture::createImage()
{
    VkPhysicalDevice phyDevice = vk::VulkanContext::GetDevice().getPhysicalDevice();

    // Create the image
    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = static_cast<uint32_t>(m_width);
    imageInfo.extent.height = static_cast<uint32_t>(m_height);
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;

    VK_CALL(vkCreateImage(m_device, &imageInfo, nullptr, &m_image));

    // Allocate the memory for the image
    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(m_device, m_image, &memRequirements);

    VkPhysicalDeviceMemoryProperties deviceProperties;
    vkGetPhysicalDeviceMemoryProperties(phyDevice, &deviceProperties);

    // Find a suitable memory type
    uint32_t memoryType = UINT32_MAX;
    for (uint32_t i = 0; i < deviceProperties.memoryTypeCount; ++i)
    {
        if (!(memRequirements.memoryTypeBits & (1 << i))) {
            continue;
        }
        else if (!(deviceProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)) {
            continue;
        }

        memoryType = i;
        break;
    }
    if (memoryType == UINT32_MAX) {
        JDL_FATAL("Failed to find a suitable memory type");
    }

    VkMemoryAllocateInfo allocInfo {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = memoryType;

    VK_CALL(vkAllocateMemory(m_device, &allocInfo, nullptr, &m_deviceMemory));
    VK_CALL(vkBindImageMemory(m_device, m_image, m_deviceMemory, 0));
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

        transitionToLayout(bufferHandle, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

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
            m_image,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            1, &region
        );

        transitionToLayout(
            bufferHandle,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        );
    }
    commandBuffer.end();
    commandBuffer.submit(queue);

    VK_CALL(vkQueueWaitIdle(queue));
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

void Texture::transitionToLayout(VkCommandBuffer commandBuffer, VkImageLayout oldLayout, VkImageLayout newLayout)
{
    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    VkImageMemoryBarrier barrier {};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = m_image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
    {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else
    {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }

    vkCmdPipelineBarrier(
        commandBuffer,
        sourceStage, destinationStage,
        0,
        0, nullptr,
        0, nullptr,
        1, &barrier
    );
}

void Texture::clearResource()
{
    if (m_image != VK_NULL_HANDLE)
    {
        vkDestroySampler(m_device, m_sampler, nullptr);
        vkFreeMemory(m_device, m_deviceMemory, nullptr);
        vkDestroyImageView(m_device, m_imageView, nullptr);
        vkDestroyImage(m_device, m_image, nullptr);
    }
}

} // namespace resource
} // namespace jdl
