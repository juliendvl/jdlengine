#include "utils/VulkanUtils.hpp"
#include "utils/Logger.hpp"

#include "core/VulkanContext.hpp"


namespace utils
{
namespace VulkanUtils
{

VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectMask)
{
    VkDevice device = core::VulkanContext::GetDevice();

    VkImageViewCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.image = image;
    createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    createInfo.format = format;
    createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.subresourceRange.aspectMask = aspectMask;
    createInfo.subresourceRange.baseMipLevel = 0;
    createInfo.subresourceRange.levelCount = 1;
    createInfo.subresourceRange.baseArrayLayer = 0;
    createInfo.subresourceRange.layerCount = 1;

    VkImageView view;
    VK_CALL(vkCreateImageView(device, &createInfo, nullptr, &view));

    return view;
}

} // namespace VulkanUtils
} // namespace utils
