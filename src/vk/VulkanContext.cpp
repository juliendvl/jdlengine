#include "vk/VulkanContext.hpp"


namespace jdl
{
namespace vk
{

VulkanContext VulkanContext::IContext;

void VulkanContext::doInit()
{
	if (m_instance) {
		return;
	}

	m_instance = std::make_unique<VulkanInstance>();
}

void VulkanContext::doDestroy()
{
	if (!m_instance) {
		return;
	}
	m_instance.reset();
}

} // namespace vk
} // namespace jdl
