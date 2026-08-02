#include "vk/vulkan_command_buffer.hpp"

#include "utils/logger.hpp"

#include "vk/vulkan_context.hpp"


namespace jdl
{
namespace vk
{

VulkanCommandBuffer::VulkanCommandBuffer(VkCommandPool command_pool)
{
	m_device = VulkanContext::GetDevice().get_device();
	m_commandPool = command_pool;

	VkCommandBufferAllocateInfo alloc_info {
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		.commandPool = command_pool,
		.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		.commandBufferCount = 1,
	};
	VK_CALL(vkAllocateCommandBuffers(m_device, &alloc_info, &m_commandBuffer));
}

VulkanCommandBuffer::~VulkanCommandBuffer() {}

void VulkanCommandBuffer::begin()
{
	VK_CALL(vkResetCommandBuffer(m_commandBuffer, 0));

	VkCommandBufferBeginInfo begin_info {
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO
	};
	VK_CALL(vkBeginCommandBuffer(m_commandBuffer, &begin_info));

	m_recording = true;
}

void VulkanCommandBuffer::end()
{
	VK_CALL(vkEndCommandBuffer(m_commandBuffer));
	m_recording = false;
}

void VulkanCommandBuffer::submit(
	VkQueue queue,
	const std::vector<VkSemaphore>& wait_semaphores,
	const std::vector<VkPipelineStageFlags>& wait_stages,
	const std::vector<VkSemaphore>& signal_semaphores,
	VkFence fence
)
{
	VkSubmitInfo submit_info {
		.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
		.waitSemaphoreCount = VK_SIZE(wait_semaphores),
		.pWaitSemaphores = VK_DATA(wait_semaphores),
		.pWaitDstStageMask = VK_DATA(wait_stages),
		.commandBufferCount = 1,
		.pCommandBuffers = &m_commandBuffer,
		.signalSemaphoreCount = VK_SIZE(signal_semaphores),
		.pSignalSemaphores = VK_DATA(signal_semaphores)
	};
	VK_CALL(vkQueueSubmit(queue, 1, &submit_info, fence));
}

void VulkanCommandBuffer::destroy()
{
	vkFreeCommandBuffers(m_device, m_commandPool, 1, &m_commandBuffer);
	m_commandBuffer = VK_NULL_HANDLE;
}

void VulkanCommandBuffer::transition_image_layout(
	VkImage image,
	VkImageLayout old_layout,
	VkImageLayout new_layout,
	VkAccessFlags2 src_access_mask,
	VkAccessFlags2 dst_access_mask,
	VkPipelineStageFlags2 src_stage_mask,
	VkPipelineStageFlags2 dst_stage_mask,
	VkImageAspectFlags aspect_mask
)
{
	VkImageMemoryBarrier2 barrier {
		.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
		.srcStageMask = src_stage_mask,
		.srcAccessMask = src_access_mask,
		.dstStageMask = dst_stage_mask,
		.dstAccessMask = dst_access_mask,
		.oldLayout = old_layout,
		.newLayout = new_layout,
		.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
		.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
		.image = image,
		.subresourceRange = {
			.aspectMask = aspect_mask,
			.baseMipLevel = 0,
			.levelCount = 1,
			.baseArrayLayer = 0,
			.layerCount = 1
		}
	};

	VkDependencyInfo dependency_info {
		.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
		.imageMemoryBarrierCount = 1,
		.pImageMemoryBarriers = &barrier
	};
	vkCmdPipelineBarrier2(m_commandBuffer, &dependency_info);
}

void VulkanCommandBuffer::bind_graphics_pipeline(VkPipeline pipeline)
{
	vkCmdBindPipeline(m_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
}

void VulkanCommandBuffer::set_viewport(
	VkOffset2D offset,
	VkExtent2D extent,
	float min_depth,
	float max_depth
)
{
	VkViewport viewport {
		.x = static_cast<float>(offset.x),
		.y = static_cast<float>(offset.y),
		.width = static_cast<float>(extent.width),
		.height = static_cast<float>(extent.height),
		.minDepth = min_depth,
		.maxDepth = max_depth
	};
	vkCmdSetViewport(m_commandBuffer, 0, 1, &viewport);
}

void VulkanCommandBuffer::set_scissor(VkOffset2D offset, VkExtent2D extent)
{
	VkRect2D scissor { .offset = offset, .extent = extent };
	vkCmdSetScissor(m_commandBuffer, 0, 1, &scissor);
}

void VulkanCommandBuffer::draw(
	uint32_t nb_vertices,
	uint32_t nb_instances,
	uint32_t first_vertex,
	uint32_t first_instance
)
{
	vkCmdDraw(m_commandBuffer, nb_vertices, nb_instances, first_vertex, first_instance);
}

} // namespace vk
} // namespace jdl
