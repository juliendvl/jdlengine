#include "vk/vk_context.hpp"


namespace jdl
{
namespace vk
{

Context::Context(const core::Window& window)
{
	m_instance = std::make_unique<Instance>(window);
	m_device = std::make_unique<Device>(*m_instance);
	m_swapchain = std::make_unique<Swapchain>(*m_instance, *m_device, window);
	m_pipeline = std::make_unique<Pipeline>(*m_device, *m_swapchain);
}

// Members are destroyed in reverse declaration order: pipeline, swapchain, device, instance.
Context::~Context() = default;

} // namespace vk
} // namespace jdl
