#pragma once

#include "Core.hpp"
#include "Events.hpp"

#include "scene/Scene.hpp"

#include "utils/NonCopyable.hpp"


namespace jdl
{
namespace core
{

struct RenderContext
{
    // Current command buffer (on which commands must be registered)
    VkCommandBuffer commandBuffer = VK_NULL_HANDLE;

    /**
     * @brief Inits the render context.
     * @param commandBuffer Current command buffer
     */
    RenderContext(VkCommandBuffer _commandBuffer);

    /**
     * @brief Updates the model matrix.
     * @param matrix The new model matrix
     */
    void updateModelMatrix(const glm::mat4& matrix);

private:
    // For internal usage only
    VkDevice m_device;
    VkPipelineLayout m_pipelineLayout;
};

class JDL_API Renderer : private NonCopyable<Renderer>
{
public:
    Renderer();
    ~Renderer();

    /**
     * @brief Maximal number of frames in-flight.
     */
    static const uint32_t kMaxFramesInFlight;

    /**
     * @brief Sets the background color.
     * @param r, g, b, a The new background color
     */
    void setBackgroundColor(float r, float g, float b, float a = 1.0f) { m_clearColor.color = {r, g, b, a}; }

    /**
     * @brief Returns the active scene.
     */
    const std::shared_ptr<scene::Scene>& getScene() const {
        return m_scene;
    }

    /**
     * @brief Sets the active scene.
     * @param scene The new scene
     */
    void setScene(const std::shared_ptr<scene::Scene>& scene);

    /**
     * @brief Renders a frame.
     */
    void renderFrame();

    /**
     * @brief Resize event callback.
     * @param event Event data
     */
    void resizeEvent(const ResizeEvent& event);

private:
    // Command buffers
    std::vector<VkCommandBuffer> m_commandBuffers;
    
    // Synchronization objects
    std::vector<VkSemaphore> m_imageAvailable;
    std::vector<VkSemaphore> m_renderFinished;
    std::vector<VkFence>     m_inFlightFrame;
    
    // Index of the currently rendered frame
    uint32_t m_frameIndex;
    
    // Clear color
    VkClearValue m_clearColor;

    // Active scene
    std::shared_ptr<scene::Scene> m_scene;

    // Indicates whether a resize has been triggered or not, meaning that the swap chain must be recreated
    bool m_resizeTriggered;

    // Allocates the command buffers
    void allocateCommandBuffers(VkDevice device);
    // Creates the synchronization objects
    void createSyncObjects(VkDevice device);
    // Records the commands on the input command buffer
    void recordCommands(VkCommandBuffer commandBuffer, uint32_t imageIndex);
};

} // namespace core
} // namespace jdl
