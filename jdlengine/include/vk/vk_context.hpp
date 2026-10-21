#pragma once

#include <memory>

#include "utils/non_copyable.hpp"

#include "vk_common.hpp"
#include "vk_device.hpp"
#include "vk_instance.hpp"
#include "vk_swapchain.hpp"


namespace jdl
{
namespace vk
{

class Context : private NonCopyable<Context>
{
public:
	/**
	 * @brief Creates the Vulkan context and its underlying objects.
	 */
	Context();

	/**
	 * @brief Destroys the Vulkan context.
	 */
	~Context();

	/**
	 * @brief Returns the Vulkan instance.
	 */
	Instance& instance() const { return *m_instance; }

	/**
	 * @brief Returns the Vulkan device.
	 */
	Device& device() const { return *m_device; }

	/**
	 * @brief Returns the Vulkan swapchain.
	 */
	Swapchain& swapchain() const { return *m_swapchain; }

private:
	std::unique_ptr<Instance> m_instance;
	std::unique_ptr<Device> m_device;
	std::unique_ptr<Swapchain> m_swapchain;
};

} // namespace vk
} // namespace jdl
