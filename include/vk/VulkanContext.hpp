#pragma once

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

private:
	static VulkanContext IContext;

	std::unique_ptr<VulkanInstance> m_instance;

	void doInit();
	void doDestroy();
};

} // namespace vk
} // namespace jdl
