#pragma once

#include "VulkanDevice.hpp"
#include "VulkanInstance.hpp"
#include "VulkanSwapChain.hpp"

#include "utils/NonCopyable.hpp"


namespace jdl
{
namespace vk
{

class VulkanContext : private NonCopyable<VulkanContext>
{
public:
	/**
	 * @brief Inits the Vulkan context (instance, device...).
	 */
	static void Init() { IContext.doInit(); }

	/**
	 * @brief Destroys the Vulkan context.
	 */
	static void Destroy() { IContext.doDestroy(); }

	/**
	 * @brief Returns the Vulkan instance.
	 */
	static VulkanInstance& GetInstance() { return *IContext.m_instance; }

	/**
	 * @brief Returns the Vulkan window surface.
	 */
	static VkSurfaceKHR GetWindowSurface() {
		return IContext.m_windowSurface;
	}

	/**
	 * @brief Returns the Vulkan device.
	 */
	static VulkanDevice& GetDevice() { return *IContext.m_device; }

	/**
	 * @brief Returns the Vulkan swap chain.
	 */
	static VulkanSwapChain& GetSwapChain() { return *IContext.m_swapChain; }

private:
	static VulkanContext IContext;

	VK_ATTR(VkSurfaceKHR, m_windowSurface);

	std::unique_ptr<VulkanInstance> m_instance;
	std::unique_ptr<VulkanDevice> m_device;
	std::unique_ptr<VulkanSwapChain> m_swapChain;

	void doInit();
	void createDefaultResources();
	void doDestroy();
};

} // namespace vk
} // namespace jdl
