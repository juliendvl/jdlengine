#pragma once

#include "utils/non_copyable.hpp"

#include <set>


namespace jdl
{
namespace vk
{

struct QueueFamilyIndices
{
	uint32_t graphics = UINT32_MAX;

	/**
	 * @brief Returns whether all the required queue family indices have been
	 * set or not.
	 */
	bool is_complete() const {
		return graphics != UINT32_MAX;
	}

	/**
	 * @brief Returns the unique queue family indices.
	 */
	std::set<uint32_t> get_unique_indices() const {
		return std::set<uint32_t>{graphics};
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
	VkPhysicalDevice get_physical_device() const {
		return m_physicalDevice;
	}

	/**
	 * @brief Returns the name of the selected physical device.
	 */
	std::string get_device_name() const;

	/**
	 * @brief Returns the selected queue family indices.
	 */
	const QueueFamilyIndices& get_queue_family_indices() const {
		return m_queueFamilyIndices;
	}

	/**
	 * @brief Returns the logical device handle.
	 */
	VkDevice get_device() const { return m_device; }

	/**
	 * @brief Returns the graphics queue handle.
	 */
	VkQueue get_graphics_queue() const { return m_graphicsQueue; }

private:
	VK_ATTR(VkPhysicalDevice, m_physicalDevice);
	VK_ATTR(VkDevice, m_device);

	QueueFamilyIndices m_queueFamilyIndices;

	VK_ATTR(VkQueue, m_graphicsQueue);

	void select_physical_device();
	void create_device();
};

} // namespace vk
} // namespace jdl
