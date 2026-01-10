#pragma once

#include "core/Core.hpp"
#include "core/Events.hpp"

#include "scene/Scene.hpp"

#include "utils/NonCopyable.hpp"

#include "VulkanBuffer.hpp"
#include "VulkanCommandBuffer.hpp"

#include <array>


namespace jdl
{
namespace vk
{

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
     * @brief Sets the rendered scene.
     */
    void setScene(const std::shared_ptr<scene::Scene>& scene) {
        m_scene = scene;
    }

    /**
     * @brief Renders a new frame.
     */
    void renderFrame();

    /**
     * @brief Waits for the renderer to finish its work.
     */
    void wait();

    /**
     * @brief Mouse press event handler.
     * @param event Event data.
     */
    void mousePressEvent(const core::MousePressEvent& event);

    /**
     * @brief Mouse release event handler.
     * @param event Event data.
     */
    void mouseReleaseEvent(const core::MouseReleaseEvent& event);

    /**
     * @brief Mouse move event handler.
     * @param event Event data.
     */
    void mouseMoveEvent(const core::MouseMoveEvent& event);

    /**
     * @brief Wheel event handler.
     * @param event Event data.
     */
    void wheelEvent(const core::WheelEvent& event);

    /**
     * @brief Resize event handler.
     * @param event Event data.
     */
    void resizeEvent(const core::ResizeEvent& event);

private:
    VkDevice m_device = VK_NULL_HANDLE;

    // Command buffers
    std::vector<VulkanCommandBuffer> m_commandBuffers;
    // Uniform buffers data
    std::vector<VkDescriptorSet> m_globalDescriptorSets;
    std::vector<std::shared_ptr<VulkanBufferWrapper>> m_globalUniformBuffers;
    // Synchronization objects
    std::vector<VkSemaphore> m_imageAcquired;
    std::vector<VkSemaphore> m_renderFinished;
    std::vector<VkFence> m_startFrame;

    // Index of the currently rendered image
    uint32_t m_currentFrame = 0;

    // Clear values (background color + depth)
    std::array<VkClearValue, 2> m_clearValues;

    // Rendered scene
    std::shared_ptr<scene::Scene> m_scene = nullptr;

    // Indicates if the swap chain must be explicitly recreated
    bool m_recreateSwapChain = false;

    void createCommandBuffers();
    void createSynchronizationObjects();
    void createUniformBuffers();

    void recordCommandBuffer(VulkanCommandBuffer& commandBuffer, uint32_t imageIndex);
};

} // namespace vk
} // namespace jdl
