#pragma once

#include "Core.hpp"

#include <map>
#include <optional>
#include <set>

#include "Pipeline.hpp"
#include "SwapChain.hpp"

#include "utils/NonCopyable.hpp"


namespace jdl
{
namespace core
{

struct QueueFamilyIndices
{
    // Graphic operations family
    std::optional<uint32_t> graphics;
    // Presentation operations family
    std::optional<uint32_t> present;

    /**
     * @brief Returns whether all the required families have been set or not.
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


struct Queue
{
    // Queue index
    uint32_t index = 0;
    // Vulkan object
    VkQueue object = VK_NULL_HANDLE;
    // Command pool
    VkCommandPool commandPool = VK_NULL_HANDLE;
};


class JDL_API VulkanContext : private NonCopyable<VulkanContext>
{
public:
    /**
     * @brief Inits the Vulkan context.
     */
    static void Init() { IContext.init(); }

    /**
     * @brief Destroys the Vulkan context.
     */
    static void Destroy() { IContext.destroy(); }

    /**
     * @brief Returns the Vulkan instance handle.
     */
    static VkInstance GetInstance() { return IContext.m_instance; }

    /**
     * @brief Returns the window surface handle.
     */
    static VkSurfaceKHR GetWindowSurface() { return IContext.m_surface; }

    /**
     * @brief Returns the selected physical device handle.
     */
    static VkPhysicalDevice GetPhysicalDevice() { return IContext.m_physicalDevice; }

    /**
     * @brief Returns the selected queue family indices.
     */
    static const QueueFamilyIndices& GetQueueFamilyIndices() { return IContext.m_queueFamilyIndices; }

    /**
     * @brief Returns the logical device handle.
     */
    static VkDevice GetDevice() { return IContext.m_device; }

    /**
     * @brief Returns the queue associated to the input family index.
     * @param index Queue family index
     */
    static const Queue& GetQueue(uint32_t index) { return IContext.m_queues.at(index); }

    /**
     * @brief Returns the graphics queue.
     */
    static const Queue& GetGraphicsQueue() { return GetQueue(*IContext.m_queueFamilyIndices.graphics); }

    /**
     * @brief Returns the present queue.
     */
    static const Queue& GetPresentQueue() { return GetQueue(*IContext.m_queueFamilyIndices.present); }

    /**
     * @brief Returns the current swap chain.
     */
    static SwapChain& GetSwapChain() { return *IContext.m_swapChain; }

    /**
     * @brief Returns the current graphics pipeline.
     */
    static Pipeline& GetPipeline() { return *IContext.m_pipeline; }

private:
    static VulkanContext IContext;

    // Vulkan instance
    VkInstance m_instance;
    // Debug messenger
    VkDebugUtilsMessengerEXT m_debugMessenger;
    // Window surface
    VkSurfaceKHR m_surface;
    // Physical device
    VkPhysicalDevice m_physicalDevice;
    // Device queue families
    QueueFamilyIndices m_queueFamilyIndices;
    // Queues
    std::map<uint32_t, Queue> m_queues;
    // Logical device
    VkDevice m_device;
    // SwapChain
    std::unique_ptr<SwapChain> m_swapChain;
    // Pipeline
    std::unique_ptr<Pipeline> m_pipeline;

    VulkanContext();

    // Inits the Vulkan context
    void init();
    // Destroys the Vulkan context
    void destroy();

    // Creates the Vulkan instance
    void createInstance();
    // Creates the debug messenger object
    void createDebugMessenger();
    // Creates the window surface
    void createWindowSurface();
    // Selects a physical device
    void selectPhysicalDevice();
    // Creates the logical device
    void createDevice();
    // Retrieves the queues and creates the associated command pools
    void setupQueues();
    // Creates the default resources
    void createDefaultResources();
    // Creates the swapchain
    void createSwapChain();
    // Creates the graphics pipeline
    void createPipeline();
};

} // namespace core
} // namespace jdl
