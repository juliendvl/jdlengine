#pragma once

#include "Core.hpp"

#include "vulkan/VulkanBuffer.hpp"


namespace jdl
{
namespace core
{

struct RenderContext
{
    // Current command buffer
    VkCommandBuffer commandBuffer = VK_NULL_HANDLE;
    // Current pipeline layout
    VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
    // Current global descriptor set
    VkDescriptorSet globalDescriptorSet = VK_NULL_HANDLE;
    // Current global uniform buffer
    std::weak_ptr<vk::VulkanBufferWrapper> globalUniformBuffer;
};

} // namespace core
} // namespace jdl
