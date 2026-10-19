#include "vk/vk_context.hpp"


namespace jdl
{
namespace vk
{

Context::Context()
{
	m_instance = std::make_unique<Instance>();
	m_device = std::make_unique<Device>(*m_instance);
}

Context::~Context()
{
	m_device.reset();
	m_instance.reset();
}

} // namespace vk
} // namespace jdl
