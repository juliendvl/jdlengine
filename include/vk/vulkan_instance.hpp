#pragma once

#include "utils/non_copyable.hpp"


namespace jdl
{
namespace vk
{

class VulkanInstance : private NonCopyable<VulkanInstance>
{
public:
    VulkanInstance();
    ~VulkanInstance();

    /**
     * @brief Returns the instance Vulkan object.
     */
    VkInstance get_handle() const { return m_instance; }

private:
    VK_ATTR(VkInstance, m_instance);
    VK_ATTR(VkDebugUtilsMessengerEXT, m_debugMessenger);

    void create_instance();
    void create_debug_messenger();
};

} // namespace vk
} // namespace jdl
