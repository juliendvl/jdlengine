#pragma once

#include "core/Core.hpp"


namespace jdl
{
namespace vk
{

class JDL_API VulkanCommandBuffer
{
public:
	/**
	 * @brief Inits the command buffer class, but does not allocate the Vulkan object.
	 */
	VulkanCommandBuffer();

	/**
	 * @brief Copy constructor.
	 * @param source The source command buffer object.
	 */
	VulkanCommandBuffer(const VulkanCommandBuffer& source);

	/**
	 * @brief Allocates the command buffer.
	 * @param commandPool The parent command pool.
	 */
	void allocate(VkCommandPool commandPool);

	/**
	 * @brief Returns the command buffer handle.
	 */
	VkCommandBuffer get() const {
		return m_commandBuffer;
	}

	/**
	 * @brief Begins the command buffer recording.
	 */
	void begin();

	/**
	 * @brief Ends the command buffer recording.
	 */
	void end();

	/**
	 * @brief Submits the command buffer to the specified queue.
	 * @param queue				The target queue.
	 * @param waitSemaphores	The semaphores to wait.
	 * @param waitStages		For each wait semaphore, the pipeline stage on which it must wait.
	 * @param signalSemaphores	The semaphores to signal.
	 * @param fence				The fence to signal.
	 */
	void submit(
		VkQueue queue,
		const std::vector<VkSemaphore>& waitSemaphores = {},
		const std::vector<VkPipelineStageFlags>& waitStages = {},
		const std::vector<VkSemaphore>& signalSemaphores = {},
		VkFence fence = VK_NULL_HANDLE
	);

private:
	VkDevice m_device = VK_NULL_HANDLE;
	VkCommandPool m_commandPool = VK_NULL_HANDLE;
	VkCommandBuffer m_commandBuffer = VK_NULL_HANDLE;
};

} // namespace vk
} // namespace jdl
