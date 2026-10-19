#pragma once

#include <set>
#include <string>

#include "utils/non_copyable.hpp"

#include "vk_common.hpp"
#include "vk_instance.hpp"


namespace jdl
{
namespace vk
{

struct QueueFamilyIndices
{
	uint32_t graphics = UINT32_MAX;

	/**
	 * @brief Returns whether all the required queue families have been found.
	 */
	bool is_complete() const {
		return graphics != UINT32_MAX;
	}

	/**
	 * @brief Returns the distinct queue family indices.
	 */
	std::set<uint32_t> unique_indices() const {
		return { graphics };
	}
};


class Device : private NonCopyable<Device>
{
public:
	/**
	 * @brief Selects a physical device (a discrete GPU if available) and creates
	 * the logical device along with its queues.
	 * @param instance The Vulkan instance used to enumerate physical devices.
	 */
	Device(const Instance& instance);

	/**
	 * @brief Destroys the logical device.
	 */
	~Device();

	/**
	 * @brief Returns the selected physical device handle.
	 */
	VkPhysicalDevice physical_device() const { return m_physicalDevice; }

	/**
	 * @brief Returns the logical device handle.
	 */
	VkDevice device() const { return m_device; }

	/**
	 * @brief Returns the name of the selected physical device.
	 */
	std::string device_name() const;

	/**
	 * @brief Returns the queue family indices used by the device.
	 */
	const QueueFamilyIndices& queue_family_indices() const { return m_queueIndices; }

	/**
	 * @brief Returns the graphics queue handle.
	 */
	VkQueue graphics_queue() const { return m_graphicsQueue; }

private:
	VK_ATTR(VkPhysicalDevice, m_physicalDevice);
	VK_ATTR(VkDevice, m_device);

	QueueFamilyIndices m_queueIndices;

	VK_ATTR(VkQueue, m_graphicsQueue);

	void select_physical_device(const Instance& instance);
	void create_device();
};

} // namespace vk
} // namespace jdl
