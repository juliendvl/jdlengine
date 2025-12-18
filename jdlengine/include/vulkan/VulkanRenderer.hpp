#pragma once

#include "core/Core.hpp"

#include "utils/NonCopyable.hpp"

#include "VulkanCommandBuffer.hpp"


namespace jdl
{
namespace vk
{

struct RenderContext
{
    // Current command buffer
    VkCommandBuffer commandBuffer = VK_NULL_HANDLE;
};


class JDL_API VulkanRenderer : private NonCopyable<VulkanRenderer>
{
public:
    /**
     * @brief Inits the renderer.
     */
    VulkanRenderer();

    /**
     * @brief Destroys the renderer.
     */
    ~VulkanRenderer();

    /**
     * @brief Renders a new frame.
     */
    void renderFrame();

private:
    VkDevice m_device = VK_NULL_HANDLE;

    // Command buffers
    std::vector<VulkanCommandBuffer> m_commandBuffers;
    // Synchronization objects
    std::vector<VkSemaphore> m_imageAcquired;
    std::vector<VkSemaphore> m_renderFinished;
    std::vector<VkFence> m_startFrame;

    // Index of the currently rendered image
    uint32_t m_currentFrame = 0;

    // Background color
    VkClearValue m_backgroundColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };

    void createCommandBuffers();
    void createSynchronizationObjects();

    void recordCommandBuffer(VulkanCommandBuffer& commandBuffer, uint32_t imageIndex);
};

} // namespace vk
} // namespace jdl
