#include "vulkan/VulkanCommandBuffer.hpp"
#include "vulkan/VulkanContext.hpp"

#include "utils/Logger.hpp"


namespace jdl
{
namespace vk
{

VulkanCommandBuffer::VulkanCommandBuffer()
{
	m_device = VulkanContext::GetDevice().get();
}

VulkanCommandBuffer::VulkanCommandBuffer(const VulkanCommandBuffer& source)
	: m_device(source.m_device)
	, m_commandPool(VK_NULL_HANDLE)
	, m_commandBuffer(VK_NULL_HANDLE)
{
	if (source.m_commandBuffer != VK_NULL_HANDLE) {
		allocate(source.m_commandPool);
	}
}

void VulkanCommandBuffer::allocate(VkCommandPool commandPool)
{
	VkCommandBufferAllocateInfo allocInfo {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = 1;

	VK_CALL(vkAllocateCommandBuffers(m_device, &allocInfo, &m_commandBuffer));
	m_commandPool = commandPool;
}

void VulkanCommandBuffer::begin()
{
	if (m_commandBuffer == VK_NULL_HANDLE)
	{
		JDL_ERROR("The command buffer has not been allocated");
		return;
	}

	VkCommandBufferBeginInfo beginInfo {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	VK_CALL(vkBeginCommandBuffer(m_commandBuffer, &beginInfo));
}

void VulkanCommandBuffer::end()
{
	if (m_commandBuffer == VK_NULL_HANDLE)
	{
		JDL_ERROR("The command buffer has not been allocated");
		return;
	}
	VK_CALL(vkEndCommandBuffer(m_commandBuffer));
}

void VulkanCommandBuffer::submit(
	VkQueue queue,
	const std::vector<VkSemaphore>& waitSemaphores,
	const std::vector<VkPipelineStageFlags>& waitStages,
	const std::vector<VkSemaphore>& signalSemaphores,
	VkFence fence
)
{
	if (m_commandBuffer == VK_NULL_HANDLE)
	{
		JDL_ERROR("The command buffer has not been allocated");
		return;
	}

	VkSubmitInfo submitInfo {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &m_commandBuffer;
	submitInfo.waitSemaphoreCount = static_cast<uint32_t>(waitSemaphores.size());
	submitInfo.pWaitSemaphores = waitSemaphores.data();
	submitInfo.pWaitDstStageMask = waitStages.data();
	submitInfo.signalSemaphoreCount = static_cast<uint32_t>(signalSemaphores.size());
	submitInfo.pSignalSemaphores = signalSemaphores.data();

	VK_CALL(vkQueueSubmit(queue, 1, &submitInfo, fence));
}

} // namespace vk
} // namespace jdl
