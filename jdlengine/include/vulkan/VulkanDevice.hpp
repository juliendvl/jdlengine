#pragma once

#include "core/Core.hpp"

#include "utils/NonCopyable.hpp"

#include <set>


namespace jdl
{
namespace vk
{

struct QueueFamilyIndices
{
    uint32_t graphics = UINT32_MAX;
    uint32_t present = UINT32_MAX;

    /**
     * @brief Returns whether all the required queue families have been found or not.
     */
    bool isComplete() const {
        return graphics != UINT32_MAX && present != UINT32_MAX;
    }

    /**
     * @brief Returns the unique queue family indices.
     */
    std::set<uint32_t> getUniqueIndices() const{
        return {graphics, present};
    }
};

class JDL_API VulkanDevice : private NonCopyable<VulkanDevice>
{
public:
    /**
     * @brief Creates the device.
     */
    VulkanDevice();

    /**
     * @brief Destroys the device.
     */
    ~VulkanDevice();

    /**
     * @brief Returns the selected physical device object.
     */
    VkPhysicalDevice getPhysicalDevice() const {
        return m_physicalDevice;
    }

    /**
     * @brief Returns the device queue family indices.
     */
    const QueueFamilyIndices& getQueueFamilyIndices() const {
        return m_queueFamilyIndices;
    }

    /**
     * @brief Returns the device object.
     */
    VkDevice get() const {
        return m_device;
    }

    /**
     * @brief Returns the graphics queue object.
     */
    VkQueue getGraphicsQueue() const {
        return m_graphicsQueue;
    }

    /**
     * @brief Returns the present queue object.
     */
    VkQueue getPresentQueue() const {
        return m_presentQueue;
    }

    /**
     * @brief Returns the command pool associated to the graphics queue.
     */
    VkCommandPool getCommandPool() const {
        return m_commandPool;
    }

private:
    // Selected physical device
    VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
    // Physical device queue families
    QueueFamilyIndices m_queueFamilyIndices;
    // Logical device
    VkDevice m_device = VK_NULL_HANDLE;
    // Queue handles
    VkQueue m_graphicsQueue = VK_NULL_HANDLE;
    VkQueue m_presentQueue = VK_NULL_HANDLE;
    // Command pool (graphics queue)
    VkCommandPool m_commandPool = VK_NULL_HANDLE;

    void selectPhysicalDevice();
    void createDevice();
    void createCommandPool();
};

} // namespace vk
} // namespace jdl
