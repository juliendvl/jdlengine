#include "vk/vulkan_renderer.hpp"

#include "utils/logger.hpp"

#include "vk/vulkan_context.hpp"


namespace jdl
{
namespace vk
{

VulkanRenderer::VulkanRenderer()
{
    VulkanContext::Init();
    m_device = VulkanContext::GetDevice().get_device();

    create_sync_objects();
    create_command_buffers();
}

VulkanRenderer::~VulkanRenderer()
{
    for (auto i = 0; i < m_imageAcquiredSemaphores.size(); ++i)
    {
        vkDestroySemaphore(m_device, m_imageAcquiredSemaphores[i], nullptr);
        vkDestroySemaphore(m_device, m_renderFinishedSemaphores[i], nullptr);
        vkDestroyFence(m_device, m_inFlightFences[i], nullptr);
    }

    VulkanContext::Destroy();
}

void VulkanRenderer::set_background_color(float r, float g, float b, float a)
{
    m_clearColor.color.float32[0] = r;
    m_clearColor.color.float32[1] = g;
    m_clearColor.color.float32[2] = b;
    m_clearColor.color.float32[3] = a;
}

void VulkanRenderer::render_frame()
{
    auto& swapchain = VulkanContext::GetSwapchain();

    VkSemaphore image_acquired = m_imageAcquiredSemaphores[m_currentImage];
    VkSemaphore render_finished = m_renderFinishedSemaphores[m_currentImage];
    VkFence in_flight = m_inFlightFences[m_currentImage];

    VK_CALL(vkWaitForFences(m_device, 1, &in_flight, VK_FALSE, UINT64_MAX));

    uint32_t image_index;
    VkResult result = swapchain.acquire_image(image_index, image_acquired);

    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        VulkanContext::RecreateSwapchain();
        return;
    }
    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        JDL_FATAL("Failed to acquire an image from the swapchain");
    }

    VK_CALL(vkResetFences(m_device, 1, &in_flight));

    // Record the command buffer
    VulkanCommandBuffer* command_buffer = m_commandBuffers[m_currentImage].get();

    command_buffer->begin();
    record_command_buffer(command_buffer, image_index);
    command_buffer->end();

    // Submit the command buffer
    command_buffer->submit(
        VulkanContext::GetDevice().get_graphics_queue(),
        { image_acquired },
        { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT },
        { render_finished },
        in_flight
    );

    // Present the image to the swapchain
    VkSwapchainKHR swapchain_handle = swapchain.get_handle();
    VkPresentInfoKHR present_info {
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &render_finished,
        .swapchainCount = 1,
        .pSwapchains = &swapchain_handle,
        .pImageIndices = &image_index
    };

    VkQueue present_queue = VulkanContext::GetDevice().get_present_queue();
    result = vkQueuePresentKHR(present_queue, &present_info);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
        VulkanContext::RecreateSwapchain();
    }
    else if (m_framebufferResized)
    {
        VulkanContext::RecreateSwapchain();
        m_framebufferResized = false;
    }

    m_currentImage = (m_currentImage + 1) % m_inFlightFences.size();
}

void VulkanRenderer::wait_idle() const
{
    auto& device = VulkanContext::GetDevice();
    device.wait_idle();
}

void VulkanRenderer::resize_event(const core::ResizeEvent& event)
{
    m_framebufferResized = true;
}

void VulkanRenderer::create_sync_objects()
{
    uint32_t nb_images = VulkanContext::GetSwapchain().get_nb_images();

    VkSemaphoreCreateInfo semaphore_info {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO
    };
    VkFenceCreateInfo fence_info {
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .flags = VK_FENCE_CREATE_SIGNALED_BIT
    };

    m_imageAcquiredSemaphores.resize(nb_images);
    m_renderFinishedSemaphores.resize(nb_images);
    m_inFlightFences.resize(nb_images);

    for (uint32_t i = 0; i < nb_images; ++i)
    {
        VK_CALL(
            vkCreateSemaphore(
                m_device, &semaphore_info, nullptr, &m_imageAcquiredSemaphores[i]
            )
        );
        VK_CALL(
            vkCreateSemaphore(
                m_device, &semaphore_info, nullptr, &m_renderFinishedSemaphores[i]
            )
        );
        VK_CALL(vkCreateFence(m_device, &fence_info, nullptr, &m_inFlightFences[i]));
    }
}

void VulkanRenderer::create_command_buffers()
{
    auto command_pool = VulkanContext::GetDevice().get_graphics_command_pool();
    auto nb_buffers = m_imageAcquiredSemaphores.size();

    m_commandBuffers.reserve(nb_buffers);
    for (auto i = 0; i < nb_buffers; ++i)
    {
        m_commandBuffers.emplace_back(
            std::make_unique<VulkanCommandBuffer>(command_pool)
        );
    }
}

void VulkanRenderer::record_command_buffer(
    VulkanCommandBuffer* command_buffer,
    uint32_t image_index
)
{
    auto& swapchain = VulkanContext::GetSwapchain();
    auto extent = swapchain.get_extent();

    // Change the image layout for rendering (color attachment)
    command_buffer->transition_image_layout(
        swapchain.get_image(image_index),
        VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        {},
        VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT,
        VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
        VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
        VK_IMAGE_ASPECT_COLOR_BIT
    );

    // Start dynamic rendering
    VkRenderingAttachmentInfo color_attachment {
        .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
        .imageView = swapchain.get_image_view(image_index),
        .imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
        .clearValue = m_clearColor
    };
    VkRenderingInfo rendering_info {
        .sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
        .renderArea = {.offset = {0, 0}, .extent = extent},
        .layerCount = 1,
        .colorAttachmentCount = 1,
        .pColorAttachments = &color_attachment
    };
    vkCmdBeginRendering(command_buffer->get(), &rendering_info);

    // Bind the graphics pipeline
    command_buffer->bind_graphics_pipeline(
        VulkanContext::GetPipeline().get_pipeline()
    );

    // Set Viewport/Scissor
    command_buffer->set_viewport({ 0, 0 }, extent, 0.0f, 1.0f);
    command_buffer->set_scissor({ 0, 0 }, extent);

    // Draw
    command_buffer->draw(3);

    // End dynamic rendering
    vkCmdEndRendering(command_buffer->get());

    // Change the image layout for swapchain presentation
    command_buffer->transition_image_layout(
        swapchain.get_image(image_index),
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
        VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT,
        {},
        VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
        VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT,
        VK_IMAGE_ASPECT_COLOR_BIT
    );
}

} // namespace vk
} // namespace jdl
