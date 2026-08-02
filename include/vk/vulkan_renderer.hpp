#pragma once

#include "vulkan_command_buffer.hpp"

#include "utils/non_copyable.hpp"


namespace jdl
{
namespace vk
{

class VulkanRenderer : private NonCopyable<VulkanRenderer>
{
public:
    VulkanRenderer();
    ~VulkanRenderer();

    /**
     * @brief Sets the background color.
     * @param r, g, b, a Background color components.
     */
    void set_background_color(float r, float g, float b, float a = 1.0f);

    /**
     * @brief Renders a new frame.
     */
    void render_frame();

    /**
     * @brief Waits for the renderer to be in idle state.
     */
    void wait_idle() const;

private:
    VK_ATTR(VkDevice, m_device);

    // Background color
    VkClearValue m_clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };

    // Synchronization objects
    std::vector<VkSemaphore> m_imageAcquiredSemaphores;
    std::vector<VkSemaphore> m_renderFinishedSemaphores;
    std::vector<VkFence> m_inFlightFences;

    std::vector<std::unique_ptr<VulkanCommandBuffer>> m_commandBuffers;

    uint32_t m_currentImage = 0;

    void create_sync_objects();
    void create_command_buffers();

    void record_command_buffer(
        VulkanCommandBuffer* command_buffer,
        uint32_t image_index
    );
};

} // namespace vk
} // namespace jdl
