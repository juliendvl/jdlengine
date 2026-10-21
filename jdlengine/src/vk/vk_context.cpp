#include "vk/vk_context.hpp"


namespace jdl
{
namespace vk
{

Context::Context()
{
	m_instance = std::make_unique<Instance>();
	m_device = std::make_unique<Device>(*m_instance);
	m_swapchain = std::make_unique<Swapchain>(*m_instance, *m_device);
}

Context::~Context()
{
	m_swapchain.reset();
	m_device.reset();
	m_instance.reset();
}

} // namespace vk
} // namespace jdl
