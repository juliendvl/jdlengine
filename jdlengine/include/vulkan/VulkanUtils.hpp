#pragma once

#include "core/Core.hpp"

#include "MemoryProperty.hpp"


namespace jdl
{
namespace vk
{
namespace VulkanUtils
{

/**
 * @brief Allocates and returns a device memory.
 * @param device            Vulkan device.
 * @param physicalDevice    Vulkan physical device.
 * @param requirements      Memory requirements.
 * @param properties        Memory properties.
 * @return The created device memory.
 */
JDL_API VkDeviceMemory AllocateDeviceMemory(
    VkDevice device,
    VkPhysicalDevice physicalDevice,
    VkMemoryRequirements requirements,
    MemoryProperty properties
);

/**
 * @brief Creates and returns an image view for the input image.
 * @param device        Vulkan device.
 * @param image         Source image.
 * @param format        Image format.
 * @param aspectMask    Image aspect mask.
 * @return The created image view.
 */
JDL_API VkImageView CreateImageView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspectMask);

/**
 * @brief Returns whether the input format includes a stencil component or not.
 */
inline bool HasStencilComponent(VkFormat format) {
    return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

/**
 * @brief Records the command allowing to change the layout of an image.
 * @param commandBuffer Command buffer on which the command must be recorded.
 * @param image         Image to be transitioned.
 * @param format        Image format.
 * @param oldLayout     Old image layout.
 * @param newLayout     New image layout.
 */
JDL_API void TransitionImageLayout(
    VkCommandBuffer commandBuffer,
    VkImage image,
    VkFormat format,
    VkImageLayout oldLayout,
    VkImageLayout newLayout
);

} // namespace VulkanUtils
} // namespace vk
} // namespace jdl
