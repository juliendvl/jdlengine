#include "vk/VulkanContext.hpp"

#include "core/Application.hpp"

#include "resource/ResourceManager.hpp"
#include "resource/Shader.hpp"


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

	m_windowSurface = core::Application::GetWindow().createWindowSurface();
	m_device = std::make_unique<VulkanDevice>();
	m_swapChain = std::make_unique<VulkanSwapChain>();

	createDefaultResources();
}

void VulkanContext::doDestroy()
{
	if (!m_instance) {
		return;
	}

	m_swapChain.reset();

	vkDestroySurfaceKHR(m_instance->get(), m_windowSurface, nullptr);

	m_device.reset();
	m_instance.reset();
}

void VulkanContext::createDefaultResources()
{
	resource::ResourceManager::Create<resource::Shader>(
		"DefaultShader",
		"shaders/default.spv"
	);
}

} // namespace vk
} // namespace jdl
