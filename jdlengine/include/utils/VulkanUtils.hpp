#pragma once

#include "core/Core.hpp"


namespace jdl
{
namespace utils
{
namespace VulkanUtils
{

/**
 * @brief  Creates and returns a device memory object.
 * @param  device             Input device
 * @param  physicalDevice     Input physical device
 * @param  memoryRequirements Resource memory requirements
 * @param  memoryProperties   Resource memory properties
 * @return The VkDeviceMemory object
 */
JDL_API VkDeviceMemory CreateDeviceMemory(
    VkDevice device,
    VkPhysicalDevice physicalDevice,
    VkMemoryRequirements memoryRequirements,
    VkMemoryPropertyFlags memoryProperties
);

/**
 * @brief  Creates the image view for the input image.
 * @param  device     Input device
 * @param  image      Source image
 * @param  format     Image format
 * @param  aspectMask Image aspect mask
 * @return The created image view.
 */
JDL_API VkImageView CreateImageView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspectMask);


/**
 * @brief Allows to record and directly execute commands.
 * Usage:
 * {
 *      VulkanCommandRecorder recorder; <- Allocates the command buffer and starts recording
 *      // ... commands ...
 * } <- Stops recording and submits the command buffer
 */
class JDL_API VulkanCommandRecorder
{
public:
    /**
     * @brief Allocates the command buffer and starts recording.
     */
    VulkanCommandRecorder();

    /**
     * @brief Stops recording, submits the command buffer and waits for its complete execution.
     */
    ~VulkanCommandRecorder();

    /**
     * @brief Command buffer on which the commands must be recorded.
     */
    VkCommandBuffer commandBuffer;
};


} // namespace VulkanUtils
} // namespace utils
} // namespace jdl
