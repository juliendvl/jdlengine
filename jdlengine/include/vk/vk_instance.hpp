#pragma once

#include "utils/non_copyable.hpp"

#include "vk_common.hpp"


namespace jdl
{
namespace vk
{

class Instance : private NonCopyable<Instance>
{
public:
	/**
	 * @brief Creates the Vulkan instance with the extensions required by
	 * the window.
	 */
	Instance();

	/**
	 * @brief Destroys the Vulkan instance.
	 */
	~Instance();

	/**
	 * @brief Returns the Vulkan instance handle.
	 */
	VkInstance handle() const { return m_instance; }

private:
	VK_ATTR(VkInstance, m_instance);
};

} // namespace vk
} // namespace jdl
