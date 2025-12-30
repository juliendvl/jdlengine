#pragma once

#include "core/Core.hpp"

#include "scene/Scene.hpp"

#include "utils/NonCopyable.hpp"

#include "VulkanBuffer.hpp"
#include "VulkanCommandBuffer.hpp"

// TODO To be removed!
#include "resource/Mesh.hpp"


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

    // Background color
    VkClearValue m_backgroundColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };

    // Rendered scene
    std::shared_ptr<scene::Scene> m_scene = nullptr;

    void createCommandBuffers();
    void createSynchronizationObjects();
    void createUniformBuffers();

    void recordCommandBuffer(VulkanCommandBuffer& commandBuffer, uint32_t imageIndex);
};

} // namespace vk
} // namespace jdl
