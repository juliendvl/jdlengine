#include "vk/vk_instance.hpp"

#include "core/application.hpp"
#include "core/window.hpp"

#include "utils/logger.hpp"


namespace jdl
{
namespace vk
{

Instance::Instance()
{
	VkApplicationInfo app_info {
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pApplicationName = core::Application::GetName(),
		.applicationVersion = VK_MAKE_VERSION(1, 0, 0),
		.pEngineName = "JDLEngine",
		.engineVersion = VK_MAKE_VERSION(1, 0, 0),
		.apiVersion = VK_API_VERSION_1_3
	};

	auto extensions = core::Window::GetInstanceExtensions();

	VkInstanceCreateInfo create_info {
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pApplicationInfo = &app_info,
		.enabledLayerCount = 0,
		.enabledExtensionCount = VK_SIZE(extensions),
		.ppEnabledExtensionNames = VK_DATA(extensions)
	};

	VK_CHECK(vkCreateInstance(&create_info, nullptr, &m_instance));
}

Instance::~Instance()
{
	vkDestroyInstance(m_instance, nullptr);
}

} // namespace vk
} // namespace jdl
