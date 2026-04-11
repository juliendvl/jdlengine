#pragma once

#include "utils/NonCopyable.hpp"

#include <set>


namespace jdl
{
namespace vk
{

struct QueueFamilyIndices
{
	uint32_t graphics = UINT32_MAX;
	
	/**
	 * @brief Returns whether all the required queue families have been set or not.
	 */
	bool isComplete() const {
		return graphics != UINT32_MAX;
	}

	/**
	 * @brief Returns the unique queue family indices.
	 */
	std::set<uint32_t> getUniqueIndices() const {
		return {graphics};
	}
};

class VulkanDevice : private NonCopyable<VulkanDevice>
{
public:
	VulkanDevice();
	~VulkanDevice();

	/**
	 * @brief Returns the selected physical device.
	 */
	VkPhysicalDevice getPhysicalDevice() const {
		return m_physicalDevice;
	}

	/**
	 * @brief Returns the selected queue family indices.
	 */
	const QueueFamilyIndices& getQueueFamilyIndices() const {
		return m_queueFamilyIndices;
	}

	/**
	 * @brief Returns the created logical device.
	 */
	VkDevice getDevice() const {
		return m_device;
	}

	/**
	 * @brief Returns the graphics queue.
	 */
	VkQueue getGraphicsQueue() const {
		return m_graphicsQueue;
	}

private:
	VK_ATTR(VkPhysicalDevice, m_physicalDevice);
	VK_ATTR(VkDevice, m_device);
	VK_ATTR(VkQueue, m_graphicsQueue);

	QueueFamilyIndices m_queueFamilyIndices;

	void selectPhysicalDevice();
	void createDevice();
};

} // namespace vk
} // namespace jdl
