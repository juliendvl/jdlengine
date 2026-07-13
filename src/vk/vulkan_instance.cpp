#include "vk/vulkan_instance.hpp"

#include "core/application.hpp"

#include "utils/logger.hpp"


namespace jdl
{
namespace vk
{

VulkanInstance::VulkanInstance()
{
    create_instance();
}

VulkanInstance::~VulkanInstance()
{
    vkDestroyInstance(m_instance, nullptr);
}

void VulkanInstance::create_instance()
{
    VkApplicationInfo appInfo {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = core::Application::GetName();
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "JDLEngine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_4;

    auto extensions = core::Application::GetWindow().GetRequiredInstanceExtensions();

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = VK_SIZE(extensions);
    createInfo.ppEnabledExtensionNames = VK_DATA(extensions);
    createInfo.enabledLayerCount = 0;

    VK_CALL(vkCreateInstance(&createInfo, nullptr, &m_instance));
}

} // namespace vk
} // namespace jdl
