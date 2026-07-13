#include "vk/vulkan_context.hpp"

#include "utils/logger.hpp"

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
}

void VulkanContext::do_destroy()
{
    if (m_instance == nullptr) {
        return;
    }

    m_instance.reset();
}

void VulkanContext::create_instance()
{
    m_instance = std::make_unique<VulkanInstance>();
    JDL_INFO("Vulkan Instance: OK");
}

} // namespace vk
} // namespace jdl
