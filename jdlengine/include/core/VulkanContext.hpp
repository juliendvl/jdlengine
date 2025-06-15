#pragma once

#include "Core.hpp"

#include "utils/NonCopyable.hpp"


namespace core
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

private:
    static VulkanContext CONTEXT;

    VkInstance m_instance = VK_NULL_HANDLE;
    VkDebugUtilsMessengerEXT m_debugMessenger = VK_NULL_HANDLE;

    void doInit();
    void doDestroy();

    void createInstance();
    void createDebugMessenger();
};

} // namespace core
