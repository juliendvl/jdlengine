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
	 * the window, then the window surface. In debug builds, also enables the
	 * validation layers and the debug messenger.
	 */
	Instance();

	/**
	 * @brief Destroys the window surface, the debug messenger (if any) and the
	 * Vulkan instance.
	 */
	~Instance();

	/**
	 * @brief Returns the Vulkan instance handle.
	 */
	VkInstance handle() const { return m_instance; }

	/**
	 * @brief Returns the window surface handle.
	 */
	VkSurfaceKHR surface() const { return m_surface; }

private:
	VK_ATTR(VkInstance, m_instance);
	// Only created in debug builds
	VK_ATTR(VkDebugUtilsMessengerEXT, m_debug_messenger);
	VK_ATTR(VkSurfaceKHR, m_surface);
};

} // namespace vk
} // namespace jdl
