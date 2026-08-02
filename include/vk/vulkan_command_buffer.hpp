#pragma once

#include "utils/non_copyable.hpp"


namespace jdl
{
namespace vk
{

class VulkanCommandBuffer
{
public:
	/**
	 * @brief Creates the command buffer wrapper.
	 * @param command_pool	The command pool with which the command buffer will
	 *						be allocated.
	 */
	VulkanCommandBuffer(VkCommandPool command_pool);

	~VulkanCommandBuffer();

	/**
	 * @brief Returns the Vulkan command buffer handle.
	 */
	VkCommandBuffer get() const { return m_commandBuffer; }

	/**
	 * @brief Starts recording the command buffer.
	 */
	void begin();

	/**
	 * @brief Returns whether the command buffer is recording or not.
	 */
	bool is_recording() const { return m_recording; }

	/**
	 * @brief Stops recording the command buffer.
	 */
	void end();

	/**
	 * @brief Submits the command buffer.
	 * @param queue The queue which will execute the command buffer.
	 * @param wait_semaphores The semaphores to wait before starting the execution.
	 * @param wait_stages The pipeline stages on which each semaphore must be waited.
	 * @param signal_semaphores The semaphores to signal after the execution.
	 * @param fence The fence to signal after the execution.
	 */
	void submit(
		VkQueue queue,
		const std::vector<VkSemaphore>& wait_semaphores = {},
		const std::vector<VkPipelineStageFlags>& wait_stages = {},
		const std::vector<VkSemaphore>& signal_semaphores = {},
		VkFence fence = VK_NULL_HANDLE
	);

	/**
	 * @brief Explicitly deallocates the command buffer.
	 */
	void destroy();

	/**
	 * @brief Records the command allowing to change an image layout.
	 * @param image Image to be updated.
	 * @param old_layout Old image layout.
	 * @param new_layout New image layout.
	 * @param src_access_mask Source access mask.
	 * @param dst_access_mask Destination access mask.
	 * @param src_stage_mask Source pipeline stage mask.
	 * @param dst_stage_mask Destination pipeline stage mask.
	 * @param aspect_mask Image aspect mask.
	 */
	void transition_image_layout(
		VkImage image,
		VkImageLayout old_layout,
		VkImageLayout new_layout,
		VkAccessFlags2 src_access_mask,
		VkAccessFlags2 dst_access_mask,
		VkPipelineStageFlags2 src_stage_mask,
		VkPipelineStageFlags2 dst_stage_mask,
		VkImageAspectFlags aspect_mask
	);

	/**
	 * @brief Records the command allowing to bind the graphics pipeline.
	 * @param pipeline Graphics pipeline object.
	 */
	void bind_graphics_pipeline(VkPipeline pipeline);

	/**
	 * @brief Records the command allowing to set the viewport.
	 * @param offset Viewport top-left corner.
	 * @param extent Viewport dimensions.
	 * @param min_depth Viewport min depth.
	 * @param max_depth Viewport max depth.
	 */
	void set_viewport(
		VkOffset2D offset,
		VkExtent2D extent,
		float min_depth,
		float max_depth
	);

	/**
	 * @brief Records the command allowing to set the scissor.
	 * @param offset Scissor top-left corner.
	 * @param extent Scissor dimensions.
	 */
	void set_scissor(VkOffset2D offset, VkExtent2D extent);

	/**
	 * @brief Records the command allowing to draw vertices.
	 * @param nb_vertices The number of vertices to draw.
	 * @param nb_instances The number of instances to draw.
	 * @param first_vertex The index of the first vertex.
	 * @param first_instance The index of the first instance.
	 */
	void draw(
		uint32_t nb_vertices,
		uint32_t nb_instances = 1,
		uint32_t first_vertex = 0,
		uint32_t first_instance = 0
	);

private:
	VK_ATTR(VkDevice, m_device);
	VK_ATTR(VkCommandPool, m_commandPool);
	VK_ATTR(VkCommandBuffer, m_commandBuffer);

	bool m_recording = false;
};

} // namespace vk
} // namespace jdl
