#include "vulkan/VulkanUtils.hpp"

#include "utils/Logger.hpp"


namespace jdl
{
namespace vk
{
namespace VulkanUtils
{

VkImageView CreateImageView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspectMask)
{
    VkImageViewCreateInfo createInfo {};
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.format = format;
    createInfo.image = image;
    createInfo.subresourceRange.aspectMask = aspectMask;
    createInfo.subresourceRange.baseArrayLayer = 0;
    createInfo.subresourceRange.baseMipLevel = 0;
    createInfo.subresourceRange.layerCount = 1;
    createInfo.subresourceRange.levelCount = 1;
    createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;

    VkImageView view;
    VK_CALL(vkCreateImageView(device, &createInfo, nullptr, &view));

    return view;
}

} // namespace VulkanUtils
} // namespace vk
} // namespace jdl
