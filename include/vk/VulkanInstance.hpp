#pragma once

#include "utils/NonCopyable.hpp"


namespace jdl
{
namespace vk
{

class VulkanInstance : private NonCopyable<VulkanInstance>
{
public:
	VulkanInstance();
	~VulkanInstance();

	/**
	 * @brief Returns the Vulkan instance handle.
	 */
	VkInstance get() const { return m_instance; }

private:
	VK_ATTR(VkInstance, m_instance);

	void createInstance();
};

} // namespace vk
} // namespace jdl
