#pragma once

#include "core/Core.hpp"


namespace utils
{
namespace VulkanUtils
{

/**
 * @brief  Creates a view for the input image.
 * @param  image Source image
 * @param  format Image format
 * @param  aspectMask View aspect mask
 * @return Created image view
 */
JDL_API VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectMask);

} // namespace VulkanUtils
} // namespace utils
