#pragma once

#include "VulkanDevice.hpp"
#include "VulkanInstance.hpp"

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
	 * @brief Returns the Vulkan device.
	 */
	static VulkanDevice& GetDevice() { return *IContext.m_device; }

private:
	static VulkanContext IContext;

	std::unique_ptr<VulkanInstance> m_instance;
	std::unique_ptr<VulkanDevice> m_device;

	void doInit();
	void doDestroy();
};

} // namespace vk
} // namespace jdl
