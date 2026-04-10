#include "vk/VulkanInstance.hpp"

#include "core/Application.hpp"

#include "utils/Logger.hpp"


namespace jdl
{
namespace vk
{

VulkanInstance::VulkanInstance()
{
	createInstance();
}

VulkanInstance::~VulkanInstance()
{
	vkDestroyInstance(m_instance, nullptr);
}

void VulkanInstance::createInstance()
{
    VkApplicationInfo appInfo {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = core::Application::GetName();
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "JDLEngine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_4;

    auto extensions = core::Application::GetWindow().GetRequiredInstanceExtensions();

    VkInstanceCreateInfo createInfo {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();
    createInfo.enabledLayerCount = 0;

    VK_CALL(vkCreateInstance(&createInfo, nullptr, &m_instance));
}

} // namespace vk
} // namespace jdl
