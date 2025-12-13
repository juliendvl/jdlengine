#pragma once

#include "core/Core.hpp"

#include "utils/NonCopyable.hpp"

#include "VulkanDevice.hpp"
#include "VulkanPipeline.hpp"
#include "VulkanSwapChain.hpp"


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

    /**
     * @brief Returns the Vulkan swap chain object.
     */
    static VulkanSwapChain& GetSwapChain() {
        return *CONTEXT.m_swapChain;
    }

    /**
     * @brief Returns the Vulkan graphics pipeline object.
     */
    static VulkanPipeline& GetPipeline() {
        return *CONTEXT.m_pipeline;
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
    // Swap Chain
    std::unique_ptr<VulkanSwapChain> m_swapChain;
    // Graphics pipeline
    std::unique_ptr<VulkanPipeline> m_pipeline;

    void doInit();
    void doDestroy();

    void createInstance();
    void createDebugMessenger();
    void createWindowSurface();
    void createDevice();
    void createSwapChain();
    void createDefaultResources();
    void createPipeline();
};

} // namespace vk
} // namespace jdl

