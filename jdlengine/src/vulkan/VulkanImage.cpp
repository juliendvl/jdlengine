#include "vulkan/VulkanImage.hpp"

#include "vulkan/MemoryProperty.hpp"
#include "vulkan/VulkanContext.hpp"
#include "vulkan/VulkanUtils.hpp"

#include "utils/Logger.hpp"


namespace jdl
{
namespace vk
{

static VkFormat s_FindDepthFormat(ImageTiling tiling)
{
    std::vector<VkFormat> candidates {
        VK_FORMAT_D32_SFLOAT,
        VK_FORMAT_D32_SFLOAT_S8_UINT,
        VK_FORMAT_D24_UNORM_S8_UINT
    };

    VkPhysicalDevice physicalDevice = VulkanContext::GetDevice().getPhysicalDevice();
    for (VkFormat format : candidates)
    {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);

        if (tiling == ImageTiling::eLinear && props.linearTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
            return format;
        }
        if (tiling == ImageTiling::eOptimal && props.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
            return format;
        }
    }

    JDL_FATAL("Cannot find an appropriate depth format");
    return VK_FORMAT_UNDEFINED;
}

VulkanImage::VulkanImage(uint32_t width, uint32_t height, ImageUsage usage, ImageTiling tiling)
{
    VkPhysicalDevice physicalDevice = VulkanContext::GetDevice().getPhysicalDevice();
    m_device = VulkanContext::GetDevice().get();

    // Find the image format from the usage
    if (usage & ImageUsage::eSampled) {
        m_format = VK_FORMAT_R8G8B8A8_SRGB;
    }
    else if (usage & ImageUsage::eDepthStencil) {
        m_format = s_FindDepthFormat(tiling);
    }

    // Create the image
    VkImageCreateInfo imageInfo {};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = m_format;
    imageInfo.tiling = static_cast<VkImageTiling>(tiling);
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = static_cast<VkImageUsageFlags>(usage);
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;

    VK_CALL(vkCreateImage(m_device, &imageInfo, nullptr, &m_image));

    // Allocate the memory for the image and bind it.
    VkMemoryRequirements requirements;
    vkGetImageMemoryRequirements(m_device, m_image, &requirements);

    m_deviceMemory = VulkanUtils::AllocateDeviceMemory(
        m_device, physicalDevice,
        requirements,
        MemoryProperty::eDeviceLocal
    );
    VK_CALL(vkBindImageMemory(m_device, m_image, m_deviceMemory, 0));

    // Create the image view
    VkImageAspectFlags aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    if (usage & ImageUsage::eDepthStencil)
    {
        aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
        if (VulkanUtils::HasStencilComponent(m_format)) {
            aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
        }
    }
    m_imageView = VulkanUtils::CreateImageView(m_device, m_image, m_format, aspectMask);
}

VulkanImage::~VulkanImage()
{
    vkDestroyImageView(m_device, m_imageView, nullptr);
    vkFreeMemory(m_device, m_deviceMemory, nullptr);
    vkDestroyImage(m_device, m_image, nullptr);
}

} // namespace vk
} // namespace jdl
