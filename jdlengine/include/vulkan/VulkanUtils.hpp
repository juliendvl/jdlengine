#pragma once

#include "core/Core.hpp"


namespace jdl
{
namespace vk
{
namespace VulkanUtils
{

/**
 * @brief Creates and returns an image view for the input image.
 * @param device        Vulkan device.
 * @param image         Source image.
 * @param format        Image format.
 * @param aspectMask    Image aspect mask.
 * @return The created image view.
 */
VkImageView CreateImageView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspectMask);

} // namespace VulkanUtils
} // namespace vk
} // namespace jdl
