#include "utils/VulkanUtils.hpp"
#include "utils/Logger.hpp"

#include <optional>

#include "core/VulkanContext.hpp"


namespace jdl
{
namespace utils
{
namespace VulkanUtils
{

VkDeviceMemory CreateDeviceMemory(VkDevice device,
                                  VkPhysicalDevice physicalDevice,
                                  VkMemoryRequirements memoryRequirements,
                                  VkMemoryPropertyFlags memoryProperties)
{
    VkPhysicalDeviceMemoryProperties deviceMemoryProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &deviceMemoryProperties);

    // Find a suitable memory type for the resource
    std::optional<uint32_t> memoryTypeIndex;
    for (uint32_t i = 0; i < deviceMemoryProperties.memoryTypeCount; ++i)
    {
        VkMemoryType memoryType = deviceMemoryProperties.memoryTypes[i];
        if (memoryRequirements.memoryTypeBits & (1 << i) && (memoryType.propertyFlags & memoryProperties) == memoryProperties)
        {
            memoryTypeIndex = i;
            break;
        }
    }
    if (!memoryTypeIndex.has_value())
    {
        JDL_ERROR("Cannot find a suitable memory type");
        return VK_NULL_HANDLE;
    }

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memoryRequirements.size;
    allocInfo.memoryTypeIndex = *memoryTypeIndex;

    VkDeviceMemory deviceMemory;
    VK_CALL(vkAllocateMemory(device, &allocInfo, nullptr, &deviceMemory));

    return deviceMemory;
}

VkImageView CreateImageView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspectMask)
{
    VkImageViewCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.image = image;
    createInfo.format = format;
    createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.subresourceRange.aspectMask = aspectMask;
    createInfo.subresourceRange.baseMipLevel = 0;
    createInfo.subresourceRange.levelCount = 1;
    createInfo.subresourceRange.baseArrayLayer = 0;
    createInfo.subresourceRange.layerCount = 1;

    VkImageView view;
    VK_CALL(vkCreateImageView(device, &createInfo, nullptr, &view));

    return view;
}


/* --- VulkanCommandRecorder CLASS --- */

VulkanCommandRecorder::VulkanCommandRecorder()
    : commandBuffer(VK_NULL_HANDLE)
{
    VkDevice device = core::VulkanContext::GetDevice();
    VkCommandPool commandPool = core::VulkanContext::GetGraphicsQueue().commandPool;

    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    VK_CALL(vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer));
    VK_CALL(vkBeginCommandBuffer(commandBuffer, &beginInfo));
}

VulkanCommandRecorder::~VulkanCommandRecorder()
{
    VkDevice device = core::VulkanContext::GetDevice();
    const auto& graphicsQueue = core::VulkanContext::GetGraphicsQueue();

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    VK_CALL(vkEndCommandBuffer(commandBuffer));
    VK_CALL(vkQueueSubmit(graphicsQueue.object, 1, &submitInfo, VK_NULL_HANDLE));
    VK_CALL(vkQueueWaitIdle(graphicsQueue.object));

    vkFreeCommandBuffers(device, graphicsQueue.commandPool, 1, &commandBuffer);
}

} // namespace VulkanUtils
} // namespace utils
} // namespace jdl
