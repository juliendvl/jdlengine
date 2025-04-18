#include "core/Renderer.hpp"
#include "core/VulkanContext.hpp"

// TODO - TO BE REMOVED
#include "core/Vertex.hpp"
#include "resource/RenderMesh.hpp"
#include "resource/ResourceManager.hpp"

#include "utils/Logger.hpp"


namespace jdl
{
namespace core
{

static resource::RenderMesh* s_Mesh = nullptr;

const uint32_t Renderer::kMaxFramesInFlight = 2;

Renderer::Renderer()
    : m_frameIndex(0)
    , m_resizeTriggered(false)
{
    VkDevice device = VulkanContext::GetDevice();

    allocateCommandBuffers(device);
    createSyncObjects(device);

    setBackgroundColor(0.01f, 0.01f, 0.01f);

    s_Mesh = resource::ResourceManager::Create<resource::RenderMesh>("MESH");
    s_Mesh->addVertices({
        {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
        {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}}
    });
    s_Mesh->addIndices({0, 1, 2, 0, 2, 3});
}

Renderer::~Renderer()
{
    VkDevice device = VulkanContext::GetDevice();
    VK_CALL(vkDeviceWaitIdle(device));

    for (auto i = 0; i < kMaxFramesInFlight; ++i)
    {
        vkDestroySemaphore(device, m_imageAvailable[i], nullptr);
        vkDestroySemaphore(device, m_renderFinished[i], nullptr);
        vkDestroyFence(device, m_inFlightFrame[i], nullptr);
    }
}

void Renderer::setScene(const std::shared_ptr<scene::Scene>& scene)
{
    m_scene = scene;
}

void Renderer::renderFrame()
{
    VkDevice device = VulkanContext::GetDevice();
    VkSwapchainKHR swapChain = VulkanContext::GetSwapChain().getObject();

    // Retrieve the objects for the current frame
    VkCommandBuffer commandBuffer = m_commandBuffers[m_frameIndex];
    VkSemaphore     imageAvailable = m_imageAvailable[m_frameIndex];
    VkSemaphore     renderFinished = m_renderFinished[m_frameIndex];
    VkFence         inFlightFence = m_inFlightFrame[m_frameIndex];

    VK_CALL(vkWaitForFences(device, 1, &inFlightFence, VK_TRUE, UINT64_MAX));

    // Retrieve an image from the swap chain
    uint32_t imageIndex;
    auto result = vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailable, VK_NULL_HANDLE, &imageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        VulkanContext::RecreateSwapChain();
        return;
    }
    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
        JDL_FATAL("Failed to acquire a swap chain image");
    }

    // The fence reset must be done after retrieving the swap chain image, to avoid deadlocks
    VK_CALL(vkResetFences(device, 1, &inFlightFence));

    // Record the command buffer
    vkResetCommandBuffer(commandBuffer, 0);
    recordCommands(commandBuffer, imageIndex);

    // Submit the command buffer
    VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &imageAvailable;
    submitInfo.pWaitDstStageMask = &waitStage;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &renderFinished;

    VkQueue graphicsQueue = VulkanContext::GetGraphicsQueue().object;
    VK_CALL(vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFence));

    // Present the image to the swap chain
    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &renderFinished;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &swapChain;
    presentInfo.pImageIndices = &imageIndex;

    VkQueue presentQueue = VulkanContext::GetPresentQueue().object;
    result = vkQueuePresentKHR(presentQueue, &presentInfo);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_resizeTriggered)
    {
        VulkanContext::RecreateSwapChain();
        m_resizeTriggered = false;
    }

    m_frameIndex = (m_frameIndex + 1) % kMaxFramesInFlight;
}

void Renderer::resizeEvent(const ResizeEvent& event)
{
    m_resizeTriggered = true;

    if (m_scene)
    {
        m_scene->resizeEvent(event);
    }
}

void Renderer::allocateCommandBuffers(VkDevice device)
{
    VkCommandPool commandPool = VulkanContext::GetGraphicsQueue().commandPool;
    
    m_commandBuffers.resize(kMaxFramesInFlight);
    for (auto i = 0; i < kMaxFramesInFlight; ++i)
    {
        VkCommandBufferAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = commandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = kMaxFramesInFlight;

        VK_CALL(vkAllocateCommandBuffers(device, &allocInfo, m_commandBuffers.data()));
    }
}

void Renderer::createSyncObjects(VkDevice device)
{
    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo = {};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    m_imageAvailable.resize(kMaxFramesInFlight);
    m_renderFinished.resize(kMaxFramesInFlight);
    m_inFlightFrame.resize(kMaxFramesInFlight);

    for (auto i = 0; i < kMaxFramesInFlight; ++i)
    {
        VK_CALL(vkCreateSemaphore(device, &semaphoreInfo, nullptr, &m_imageAvailable[i]));
        VK_CALL(vkCreateSemaphore(device, &semaphoreInfo, nullptr, &m_renderFinished[i]));
        VK_CALL(vkCreateFence(device, &fenceInfo, nullptr, &m_inFlightFrame[i]));
    }
}

void Renderer::recordCommands(VkCommandBuffer commandBuffer, uint32_t imageIndex)
{
    auto& swapChain = VulkanContext::GetSwapChain();
    auto& pipeline = VulkanContext::GetPipeline();

    RenderContext context;
    context.commandBuffer = commandBuffer;

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    
    VK_CALL(vkBeginCommandBuffer(commandBuffer, &beginInfo));
    {
        VkExtent2D extent = swapChain.getExtent();

        VkRenderPassBeginInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = pipeline.getRenderPass();
        renderPassInfo.framebuffer = swapChain.getFramebuffer(imageIndex);
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = extent;
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &m_clearColor;

        VkViewport viewport = {};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(extent.width);
        viewport.height = static_cast<float>(extent.height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = extent;

        // Start the render pass
        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        // Bind the pipeline
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.getObject());
        // Set the viewport and scissor
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
        // Draw
        if (m_scene)
        {
            m_scene->render(context);
        }
        s_Mesh->render(context);
        // End the render pass
        vkCmdEndRenderPass(commandBuffer);
    }
    VK_CALL(vkEndCommandBuffer(commandBuffer));
}

} // namespace core
} // namespace jdl
