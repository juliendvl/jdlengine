#include "vk/vulkan_context.hpp"

#include "core/window.hpp"

#include "utils/logger.hpp"

#include "vk/vulkan_device.hpp"
#include "vk/vulkan_instance.hpp"


namespace jdl
{
namespace vk
{

VulkanContext VulkanContext::s_Context;

void VulkanContext::do_init()
{
    if (m_instance != nullptr) {
        return;
    }

    create_instance();
    create_window_surface();
    create_device();
}

void VulkanContext::do_destroy()
{
    if (m_instance == nullptr) {
        return;
    }

    vkDestroySurfaceKHR(m_instance->get_handle(), m_windowSurface, nullptr);
    
    m_device.reset();
    m_instance.reset();
}

void VulkanContext::create_instance()
{
    m_instance = std::make_unique<VulkanInstance>();
    JDL_INFO("Vulkan Instance: OK");
}

void VulkanContext::create_window_surface()
{
    m_windowSurface = core::Window::Get().create_window_surface(
        m_instance->get_handle()
    );
    JDL_INFO("Vulkan Window Surface: OK");
}

void VulkanContext::create_device()
{
    m_device = std::make_unique<VulkanDevice>();
    JDL_INFO("Vulkan Device: OK ({})", m_device->get_device_name());
}

} // namespace vk
} // namespace jdl
