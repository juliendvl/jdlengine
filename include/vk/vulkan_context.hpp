#pragma once

#include "vulkan_device.hpp"
#include "vulkan_instance.hpp"
#include "vulkan_swapchain.hpp"


namespace jdl
{
namespace vk
{

class VulkanContext
{
public:
    /**
     * @brief Inits the Vulkan context. Must be called only once.
     */
    static void Init() { s_Context.do_init(); }

    /**
     * @brief Destroys the Vulkan context. Must be called only once.
     */
    static void Destroy() { s_Context.do_destroy(); }

    /**
     * @brief Returns the Vulkan instance object.
     */
    static VulkanInstance& GetInstance() { return *s_Context.m_instance; }

    /**
     * @brief Returns the Vulkan window surface handle.
     */
    static VkSurfaceKHR GetWindowSurface() { return s_Context.m_windowSurface; }

    /**
     * @brief Returns the Vulkan device object.
     */
    static VulkanDevice& GetDevice() { return *s_Context.m_device; }

    /**
     * @brief Returns the Vulkan swapchain object.
     */
    static VulkanSwapchain& GetSwapchain() { return *s_Context.m_swapchain; }

private:
    static VulkanContext s_Context;

    std::unique_ptr<VulkanInstance> m_instance;
    std::unique_ptr<VulkanDevice> m_device;
    std::unique_ptr<VulkanSwapchain> m_swapchain;

    VK_ATTR(VkSurfaceKHR, m_windowSurface);

    void do_init();
    void do_destroy();

    void create_instance();
    void create_window_surface();
    void create_device();
    void create_swapchain();
};

} // namespace vk
} // namespace jdl
