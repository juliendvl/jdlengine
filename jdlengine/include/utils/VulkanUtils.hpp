#pragma once

#include "core/Core.hpp"


namespace jdl
{
namespace utils
{
namespace VulkanUtils
{

/**
 * @brief  Creates the image view for the input image.
 * @param  device     Input device
 * @param  image      Source image
 * @param  format     Image format
 * @param  aspectMask Image aspect mask
 * @return The created image view.
 */
JDL_API VkImageView CreateImageView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspectMask);


} // namespace VulkanUtils
} // namespace utils
} // namespace jdl
