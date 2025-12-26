#include "vulkan/VulkanRenderer.hpp"
#include "vulkan/VulkanContext.hpp"

#include "utils/Logger.hpp"


namespace jdl
{
namespace vk
{

// Maximum number of frames in flight
static const uint32_t sFramesInFlight = 2;


VulkanRenderer::VulkanRenderer()
{
    // Init the VulkanContext
    VulkanContext::Init();
    m_device = VulkanContext::GetDevice().get();

    createCommandBuffers();
    createSynchronizationObjects();
}

VulkanRenderer::~VulkanRenderer()
{
    for (auto i = 0; i < sFramesInFlight; ++i)
    {
        vkDestroySemaphore(m_device, m_imageAcquired[i], nullptr);
        vkDestroySemaphore(m_device, m_renderFinished[i], nullptr);
        vkDestroyFence(m_device, m_startFrame[i], nullptr);
    }

    // Destroy the VulkanContext
    VulkanContext::Destroy();
}

void VulkanRenderer::renderFrame()
{
    VulkanSwapChain& swapChain = VulkanContext::GetSwapChain();

    // Get the data for the current frame.
    VulkanCommandBuffer& commandBuffer = m_commandBuffers[m_currentFrame];
    VkSemaphore imageAcquired = m_imageAcquired[m_currentFrame];
    VkSemaphore renderFinished = m_renderFinished[m_currentFrame];
    VkFence startFrame = m_startFrame[m_currentFrame];

    // Wait for the current frame to be renderable.
    VK_CALL(vkWaitForFences(m_device, 1, &startFrame, VK_TRUE, UINT64_MAX));

    // Retrieve an image from the swap chain.
    uint32_t imageIndex = 0;
    VkResult result = swapChain.getImage(imageIndex, imageAcquired);
    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        VulkanContext::RecreateSwapChain();
        return;
    }
    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        JDL_FATAL("Failed to retrieve the swap chain image.");
    }

    // Reset the fence
    VK_CALL(vkResetFences(m_device, 1, &startFrame));

    // Record the current command buffer
    recordCommandBuffer(commandBuffer, imageIndex);

    // Submit the current command buffer
    commandBuffer.submit(
        VulkanContext::GetDevice().getGraphicsQueue(),
        { imageAcquired },
        { VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT },
        { renderFinished },
        startFrame
    );

    // Present the image to the swap chain
    result = swapChain.presentImage(
        VulkanContext::GetDevice().getPresentQueue(),
        imageIndex,
        { renderFinished }
    );
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
        VulkanContext::RecreateSwapChain();
    }
    else if (result != VK_SUCCESS) {
        JDL_FATAL("Failed to present an image to the swap chain");
    }

    m_currentFrame = (m_currentFrame + 1) % sFramesInFlight;
}

void VulkanRenderer::wait()
{
    vkDeviceWaitIdle(m_device);
}

void VulkanRenderer::createCommandBuffers()
{
    VkCommandPool commandPool = VulkanContext::GetDevice().getCommandPool();

    m_commandBuffers.resize(sFramesInFlight);
    for (VulkanCommandBuffer& commandBuffer : m_commandBuffers) {
        commandBuffer.allocate(commandPool);
    }
}

void VulkanRenderer::createSynchronizationObjects()
{
    m_imageAcquired.resize(sFramesInFlight);
    m_renderFinished.resize(sFramesInFlight);
    m_startFrame.resize(sFramesInFlight);

    VkSemaphoreCreateInfo semaphoreInfo {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo {};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (auto i = 0; i < sFramesInFlight; ++i)
    {
        VK_CALL(vkCreateSemaphore(m_device, &semaphoreInfo, nullptr, &m_imageAcquired[i]));
        VK_CALL(vkCreateSemaphore(m_device, &semaphoreInfo, nullptr, &m_renderFinished[i]));
        VK_CALL(vkCreateFence(m_device, &fenceInfo, nullptr, &m_startFrame[i]));
    }
}

void VulkanRenderer::recordCommandBuffer(VulkanCommandBuffer& commandBuffer, uint32_t imageIndex)
{
    VkCommandBuffer bufferHandle = commandBuffer.get();
    VK_CALL(vkResetCommandBuffer(bufferHandle, 0));

    commandBuffer.begin();
    {
        core::RenderContext context;
        context.commandBuffer = bufferHandle;
        context.pipelineLayout = VulkanContext::GetPipeline().getPipelineLayout();

        VulkanPipeline& pipeline = VulkanContext::GetPipeline();
        VulkanSwapChain& swapChain = VulkanContext::GetSwapChain();

        VkExtent2D extent = swapChain.getExtent();

        // Render Pass start
        VkRenderPassBeginInfo renderPassInfo {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = pipeline.getRenderPass();
        renderPassInfo.framebuffer = swapChain.getFramebuffer(imageIndex);
        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = extent;
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &m_backgroundColor;

        vkCmdBeginRenderPass(bufferHandle, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        // Pipeline selection
        vkCmdBindPipeline(bufferHandle, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.getHandle());

        // Viewport/Scissor
        VkViewport viewport {};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(extent.width);
        viewport.height = static_cast<float>(extent.height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor {};
        scissor.offset = { 0, 0 };
        scissor.extent = extent;

        vkCmdSetViewport(bufferHandle, 0, 1, &viewport);
        vkCmdSetScissor(bufferHandle, 0, 1, &scissor);

        // Draw the scene content
        if (m_scene) {
            m_scene->render(context);
        }

        // Render pass end
        vkCmdEndRenderPass(bufferHandle);
    }
    commandBuffer.end();
}

} // namespace vk
} // namespace jdl
