#pragma once


namespace jdl
{
namespace vk
{

class VulkanDevice;
class VulkanInstance;

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
     * @brief Returns the Vulkan device object.
     */
    static VulkanDevice& GetDevice() { return *s_Context.m_device; }

private:
    static VulkanContext s_Context;

    std::unique_ptr<VulkanInstance> m_instance;
    std::unique_ptr<VulkanDevice> m_device;

    void do_init();
    void do_destroy();

    void create_instance();
    void create_device();
};

} // namespace vk
} // namespace jdl
