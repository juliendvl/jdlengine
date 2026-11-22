#pragma once

#include <memory>

#include "utils/non_copyable.hpp"

#include "vk_common.hpp"
#include "vk_device.hpp"
#include "vk_instance.hpp"
#include "vk_pipeline.hpp"
#include "vk_swapchain.hpp"


namespace jdl
{
namespace vk
{

class Context : private utils::NonCopyable<Context>
{
public:
	/**
	 * @brief Creates the Vulkan context and its underlying objects.
	 * @param window The window to render to. It must outlive the context.
	 */
	explicit Context(const core::Window& window);

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

	/**
	 * @brief Returns the Vulkan graphics pipeline.
	 */
	Pipeline& pipeline() const { return *m_pipeline; }

private:
	std::unique_ptr<Instance> m_instance;
	std::unique_ptr<Device> m_device;
	std::unique_ptr<Swapchain> m_swapchain;
	std::unique_ptr<Pipeline> m_pipeline;
};

} // namespace vk
} // namespace jdl
