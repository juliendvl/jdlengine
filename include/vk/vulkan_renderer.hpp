#pragma once

#include "vulkan_command_buffer.hpp"

#include "core/events.hpp"

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

    /**
     * @brief Resize event handler.
     * @param event Event data.
     */
    void resize_event(const core::ResizeEvent& event);

private:
    VK_ATTR(VkDevice, m_device);

    // Background color
    VkClearValue m_clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };

    // Synchronization objects (one for each in-flight frame)
    std::vector<VkSemaphore> m_imageAcquiredSemaphores;
    std::vector<VkSemaphore> m_renderFinishedSemaphores;
    std::vector<VkFence> m_inFlightFences;

    // Command buffers (one for each in-flight frame)
    std::vector<std::unique_ptr<VulkanCommandBuffer>> m_commandBuffers;

    // Index of the current in-flight frame
    uint32_t m_currentImage = 0;

    // Indicates that the framebuffer has been resized (swapchain is dirty)
    bool m_framebufferResized = false;

    void create_sync_objects();
    void create_command_buffers();

    void record_command_buffer(
        VulkanCommandBuffer* command_buffer,
        uint32_t image_index
    );
};

} // namespace vk
} // namespace jdl
