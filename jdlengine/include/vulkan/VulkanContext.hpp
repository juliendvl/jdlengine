#pragma once

#include "core/Core.hpp"

#include "utils/NonCopyable.hpp"

#include "VulkanDevice.hpp"


namespace jdl
{
namespace vk
{

class JDL_API VulkanContext : private NonCopyable<VulkanContext>
{
public:
    /**
     * @brief Inits the Vulkan context.
     */
    static void Init() {
        CONTEXT.doInit();
    }

    /**
     * @brief Destroys the Vulkan context.
     */
    static void Destroy() {
        CONTEXT.doDestroy();
    }

    /**
     * @brief Returns the Vulkan instance object.
     */
    static VkInstance GetInstance() {
        return CONTEXT.m_instance;
    }

    /**
     * @brief Returns the Vulkan device object.
     */
    static VulkanDevice& GetDevice() {
        return *CONTEXT.m_device;
    }

    /**
     * @brief Returns the window surface handle.
     */
    static VkSurfaceKHR GetWindowSurface() {
        return CONTEXT.m_windowSurface;
    }

private:
    static VulkanContext CONTEXT;

    // Vulkan instance
    VkInstance m_instance = VK_NULL_HANDLE;
    // Debug Messenger
    VkDebugUtilsMessengerEXT m_debugMessenger = VK_NULL_HANDLE;
    // Window surface
    VkSurfaceKHR m_windowSurface = VK_NULL_HANDLE;

    // Device
    std::unique_ptr<VulkanDevice> m_device;

    void doInit();
    void doDestroy();

    void createInstance();
    void createDebugMessenger();
    void createWindowSurface();
    void createDevice();
};

} // namespace vk
} // namespace jdl

