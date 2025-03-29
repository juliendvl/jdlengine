#include "core/Buffers.hpp"

#include "core/VulkanContext.hpp"

#include "utils/Logger.hpp"
#include "utils/VulkanUtils.hpp"


namespace jdl
{
namespace core
{

/* --- VulkanBuffer CLASS --- */

VulkanBuffer::VulkanBuffer(uint64_t size, BufferUsage usage, MemoryProperty memoryProperties)
    : m_size(size)
    , m_usage(usage)
    , m_memoryProperties(memoryProperties)
    , m_buffer(VK_NULL_HANDLE)
    , m_deviceMemory(VK_NULL_HANDLE)
{
    VkDevice device = VulkanContext::GetDevice();

    createBuffer(device);
    allocateMemory(device);
}

VulkanBuffer::~VulkanBuffer()
{
    VkDevice device = VulkanContext::GetDevice();

    vkDestroyBuffer(device, m_buffer, nullptr);
    vkFreeMemory(device, m_deviceMemory, nullptr);
}

void VulkanBuffer::setData(void* data, uint64_t size, uint64_t offset)
{
    VkDevice device = VulkanContext::GetDevice();

    if (size == 0)
    {
        size = getSize();
    }

    void* bufferPtr;
    VK_CALL(vkMapMemory(device, m_deviceMemory, offset, size, 0, &bufferPtr));
    {
        memcpy(bufferPtr, data, size);
    }
    vkUnmapMemory(device, m_deviceMemory);
}

void VulkanBuffer::createBuffer(VkDevice device)
{
    VkBufferCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    createInfo.size = m_size;
    createInfo.usage = static_cast<VkBufferUsageFlags>(m_usage);
    createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VK_CALL(vkCreateBuffer(device, &createInfo, nullptr, &m_buffer));
}

void VulkanBuffer::allocateMemory(VkDevice device)
{
    VkPhysicalDevice physicalDevice = VulkanContext::GetPhysicalDevice();

    VkMemoryRequirements memoryRequirements;
    vkGetBufferMemoryRequirements(device, m_buffer, &memoryRequirements);

    m_deviceMemory = utils::VulkanUtils::CreateDeviceMemory(device, physicalDevice, memoryRequirements, m_memoryProperties);
    if (m_deviceMemory == VK_NULL_HANDLE)
    {
        JDL_FATAL("Failed to create the device memory object");
    }
    VK_CALL(vkBindBufferMemory(device, m_buffer, m_deviceMemory, 0));
}


/* --- Buffer CLASS --- */

Buffer::Buffer(uint64_t size, BufferUsage usage)
    : m_usage(usage)
    , m_stagingBuffer(size, BufferUsage::kTransferSrc, MemoryProperty::kHostVisible | MemoryProperty::kHostCoherent)
    , m_deviceBuffer(size, usage | BufferUsage::kTransferDst, MemoryProperty::kDeviceLocal)
{}

void Buffer::setData(void* data, uint64_t size, uint64_t offset)
{
    if (size == 0)
    {
        size = getSize();
    }

    // Update the staging buffer
    m_stagingBuffer.setData(data, size, offset);

    // Copy the data to the device buffer
    {
        VkBufferCopy copyRegion = {};
        copyRegion.srcOffset = offset;
        copyRegion.dstOffset = offset;
        copyRegion.size = size;

        utils::VulkanUtils::VulkanCommandRecorder recorder;
        vkCmdCopyBuffer(recorder.commandBuffer, m_stagingBuffer.getBuffer(), m_deviceBuffer.getBuffer(), 1, &copyRegion);
    }
}


/* --- VertexBuffer CLASS --- */

VertexBuffer::VertexBuffer(uint64_t size)
    : Buffer(size, BufferUsage::kVertex)
{}

void VertexBuffer::bind(VkCommandBuffer commandBuffer)
{
    VkBuffer buffers[] = {getDeviceBuffer().getBuffer()};
    VkDeviceSize offsets[] = {0};

    vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
}


/* --- IndexBuffer CLASS --- */

IndexBuffer::IndexBuffer(uint64_t size)
    : Buffer(size, BufferUsage::kIndex)
{}

void IndexBuffer::bind(VkCommandBuffer commandBuffer)
{
    vkCmdBindIndexBuffer(commandBuffer, getDeviceBuffer().getBuffer(), 0, VK_INDEX_TYPE_UINT32);
}

} // namespace core
} // namespace jdl
