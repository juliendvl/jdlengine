#pragma once

#include "Core.hpp"

#include "utils/NonCopyable.hpp"


namespace jdl
{
namespace core
{

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

private:
    static VulkanContext IContext;

    // Vulkan instance
    VkInstance m_instance;
    // Debug messenger
    VkDebugUtilsMessengerEXT m_debugMessenger;

    VulkanContext();

    // Inits the Vulkan context
    void init();
    // Destroys the Vulkan context
    void destroy();

    // Creates the Vulkan instance
    void createInstance();
    // Creates the debug messenger object
    void createDebugMessenger();
};

} // namespace core
} // namespace jdl
