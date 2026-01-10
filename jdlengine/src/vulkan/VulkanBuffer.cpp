#include "vulkan/VulkanBuffer.hpp"
#include "vulkan/VulkanCommandBuffer.hpp"
#include "vulkan/VulkanContext.hpp"

#include "utils/Logger.hpp"


namespace jdl
{
namespace vk
{

// --- VulkanBufferWrapper CLASS ---

VulkanBufferWrapper::VulkanBufferWrapper(uint64_t size, BufferUsage usages, MemoryProperty memoryProperties)
    : m_size(size)
{
    assert(size > 0 && "Buffer size must be > 0");
    m_device = VulkanContext::GetDevice().get();

    createBuffer(usages);
    allocateMemory(memoryProperties);
}

VulkanBufferWrapper::~VulkanBufferWrapper()
{
    vkFreeMemory(m_device, m_deviceMemory, nullptr);
    vkDestroyBuffer(m_device, m_buffer, nullptr);
}

void VulkanBufferWrapper::setData(const void* data, uint64_t size, uint64_t offset)
{
    size = size > 0 ? size : m_size;

    void* dataPtr;
    VK_CALL(vkMapMemory(m_device, m_deviceMemory, offset, size, 0, &dataPtr));
    {
        memcpy(dataPtr, data, size);
    }
    vkUnmapMemory(m_device, m_deviceMemory);
}

void VulkanBufferWrapper::createBuffer(BufferUsage usage)
{
    VkBufferCreateInfo bufferInfo {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = m_size;
    bufferInfo.usage = static_cast<VkBufferUsageFlags>(usage);
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VK_CALL(vkCreateBuffer(m_device, &bufferInfo, nullptr, &m_buffer));
}

void VulkanBufferWrapper::allocateMemory(MemoryProperty memoryProperties)
{
    VkPhysicalDevice phyDevice = VulkanContext::GetDevice().getPhysicalDevice();

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(m_device, m_buffer, &memRequirements);

    VkPhysicalDeviceMemoryProperties deviceProperties;
    vkGetPhysicalDeviceMemoryProperties(phyDevice, &deviceProperties);

    // Find a suitable memory type
    uint32_t memoryType = UINT32_MAX;
    for (uint32_t i = 0; i < deviceProperties.memoryTypeCount; ++i)
    {
        if (!(memRequirements.memoryTypeBits & (1 << i))) {
            continue;
        }
        else if ((deviceProperties.memoryTypes[i].propertyFlags & memoryProperties) != memoryProperties) {
            continue;
        }

        memoryType = i;
        break;
    }
    if (memoryType == UINT32_MAX) {
        JDL_FATAL("Failed to find a suitable memory type.");
    }

    VkMemoryAllocateInfo allocInfo {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = memoryType;

    VK_CALL(vkAllocateMemory(m_device, &allocInfo, nullptr, &m_deviceMemory));
    VK_CALL(vkBindBufferMemory(m_device, m_buffer, m_deviceMemory, 0));
}


// --- VulkanBuffer CLASS ---

VulkanBuffer::VulkanBuffer(uint64_t size, BufferUsage usages)
    : m_stagingBuffer(size, BufferUsage::eBufferTransferSrc, MemoryProperty::eHostCoherent | MemoryProperty::eHostVisible)
    , m_deviceBuffer(size, usages | BufferUsage::eBufferTransferDst, MemoryProperty::eDeviceLocal)
{}

void VulkanBuffer::setData(const void* data, uint64_t size, uint64_t offset)
{
    // Update the staging buffer
    m_stagingBuffer.setData(data, size, offset);

    // Copy the data to the device buffer.
    VulkanCommandBuffer commandBuffer;
    commandBuffer.allocate(VulkanContext::GetDevice().getCommandPool());
    commandBuffer.begin();
    {
        VkBufferCopy copyRegion {};
        copyRegion.srcOffset = offset;
        copyRegion.dstOffset = offset;
        copyRegion.size = getSize();

        vkCmdCopyBuffer(commandBuffer.get(), m_stagingBuffer.get(), m_deviceBuffer.get(), 1, &copyRegion);
    }
    commandBuffer.end();

    VkQueue queue = VulkanContext::GetDevice().getGraphicsQueue();
    commandBuffer.submit(queue);
    VK_CALL(vkQueueWaitIdle(queue));

    commandBuffer.destroy();
}


// --- VulkanVertexBuffer CLASS ---

VulkanVertexBuffer::VulkanVertexBuffer(uint64_t size)
    : VulkanBuffer(size, BufferUsage::eVertexBuffer)
{}

void VulkanVertexBuffer::bind(VkCommandBuffer commandBuffer)
{
    VkBuffer buffers[] = { getDeviceBuffer().get() };
    VkDeviceSize offsets[] = { 0 };

    vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
}


// --- VulkanIndexBuffer CLASS ---

VulkanIndexBuffer::VulkanIndexBuffer(uint64_t size)
    : VulkanBuffer(size, BufferUsage::eIndexBuffer)
{}

void VulkanIndexBuffer::bind(VkCommandBuffer commandBuffer)
{
    vkCmdBindIndexBuffer(commandBuffer, getDeviceBuffer().get(), 0, VK_INDEX_TYPE_UINT32);
}

} // namespace vk
} // namespace jdl
