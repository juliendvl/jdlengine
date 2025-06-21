#pragma once

#include "Core.hpp"

#include <optional>
#include <set>

#include "utils/NonCopyable.hpp"


namespace core
{

struct QueueFamilyIndices
{
    std::optional<uint32_t> graphics;
    std::optional<uint32_t> present;

    /**
     * @brief Returns whether all the required queue family indices have been set or not.
     */
    bool isComplete() const {
        return graphics.has_value() && present.has_value();
    }

    /**
     * @brief Returns the queue family unique indices.
     */
    std::set<uint32_t> getUniqueIndices() const
    {
        std::set<uint32_t> indices;
        if (isComplete())
        {
            indices.insert(*graphics);
            indices.insert(*present);
        }
        return indices;
    }
};


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
     * @brief Returns the Vulkan instance.
     */
    static VkInstance GetInstance() {
        return CONTEXT.m_instance;
    }

    /**
     * @brief Returns the window surface object.
     */
    static VkSurfaceKHR GetWindowSurface() {
        return CONTEXT.m_windowSurface;
    }

    /**
     * @brief Returns the selected physical device.
     */
    static VkPhysicalDevice GetPhysicalDevice() {
        return CONTEXT.m_physicalDevice;
    }

    /**
     * @brief Returns the current logical device.
     */
    static VkDevice GetDevice() {
        return CONTEXT.m_device;
    }

    /**
     * @brief Returns the graphics queue handle.
     */
    static VkQueue GetGraphicsQueue() {
        return CONTEXT.m_graphicsQueue;
    }

    /**
     * @brief Returns the present queue handle.
     */
    static VkQueue GetPresentQueue() {
        return CONTEXT.m_presentQueue;
    }

    /**
     * @brief Returns the selected queue family indices.
     */
    static const QueueFamilyIndices& GetQueueFamilyIndices() {
        return CONTEXT.m_queueFamilyIndices;
    }

private:
    static VulkanContext CONTEXT;

    VkInstance m_instance = VK_NULL_HANDLE;
    VkDebugUtilsMessengerEXT m_debugMessenger = VK_NULL_HANDLE;
    VkSurfaceKHR m_windowSurface = VK_NULL_HANDLE;

    VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
    VkDevice m_device = VK_NULL_HANDLE;

    VkQueue m_graphicsQueue = VK_NULL_HANDLE;
    VkQueue m_presentQueue = VK_NULL_HANDLE;

    QueueFamilyIndices m_queueFamilyIndices;

    void doInit();
    void doDestroy();

    void createInstance();
    void createDebugMessenger();
    void createWindowSurface();
    void selectPhysicalDevice();
    void createDevice();
};

} // namespace core
