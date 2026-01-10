#pragma once

#include "core/Core.hpp"

#include "utils/NonCopyable.hpp"

#include "MemoryProperty.hpp"


namespace jdl
{
namespace vk
{

enum BufferUsage
{
    eBufferTransferSrc  = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
    eBufferTransferDst  = VK_BUFFER_USAGE_TRANSFER_DST_BIT,
    eUniformBuffer      = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
    eIndexBuffer        = VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
    eVertexBuffer       = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT
};

inline BufferUsage operator&(BufferUsage a, BufferUsage b) { return (BufferUsage)((int)a & (int)b); }
inline BufferUsage operator|(BufferUsage a, BufferUsage b) { return (BufferUsage)((int)a | (int)b); }

inline BufferUsage& operator&=(BufferUsage& a, BufferUsage b)
{
    a = a & b;
    return a;
}

inline BufferUsage& operator|=(BufferUsage& a, BufferUsage b)
{
    a = a | b;
    return a;
}


class JDL_API VulkanBufferWrapper : private NonCopyable<VulkanBufferWrapper>
{
public:
    /**
     * @brief Creates the Vulkan buffer wrapper object.
     * @param size              Buffer size in bytes.
     * @param usages            Buffer usages.
     * @param memoryProperties  Buffer memory properties.
     */
    VulkanBufferWrapper(uint64_t size, BufferUsage usages, MemoryProperty memoryProperties);

    /**
     * @brief Destroys the Vulkan buffer wrapper object.
     */
    ~VulkanBufferWrapper();

    /**
     * @brief Returns the buffer size, in bytes.
     */
    size_t getSize() const {
        return m_size;
    }

    /**
     * @brief Returns the inner Vulkan buffer handle.
     */
    VkBuffer get() const {
        return m_buffer;
    }

    /**
     * @brief Updates the buffer data.
     * @param data      The new data.
     * @param size      Data size. Set to 0 to update the whole buffer.
     * @param offset    Buffer offset.
     */
    void setData(const void* data, uint64_t size = 0, uint64_t offset = 0);

private:
    uint64_t m_size = 0;

    VkDevice m_device = VK_NULL_HANDLE;
    VkBuffer m_buffer = VK_NULL_HANDLE;
    VkDeviceMemory m_deviceMemory = VK_NULL_HANDLE;

    void createBuffer(BufferUsage usage);
    void allocateMemory(MemoryProperty memoryProperties);
};


class JDL_API VulkanBuffer : private NonCopyable<VulkanBuffer>
{
public:
    /**
     * @brief Creates the Vulkan buffer.
     * @param size      Buffer size, in bytes.
     * @param usages    Buffer usages.
     */
    VulkanBuffer(uint64_t size, BufferUsage usages);

    /**
     * @brief Returns the buffer size in bytes.
     */
    uint64_t getSize() const {
        return m_stagingBuffer.getSize();
    }

    /**
     * @brief Returns the staging buffer object.
     */
    VulkanBufferWrapper& getStagingBuffer() {
        return m_stagingBuffer;
    }

    /**
     * @brief Returns the device buffer object.
     */
    VulkanBufferWrapper& getDeviceBuffer() {
        return m_deviceBuffer;
    }

    /**
     * @brief Updates the buffer data.
     * @param data      The new data.
     * @param size      Data size. Set to 0 to update the whole buffer.
     * @param offset    Buffer offset.
     */
    void setData(const void* data, uint64_t size = 0, uint64_t offset = 0);

private:
    VulkanBufferWrapper m_stagingBuffer;
    VulkanBufferWrapper m_deviceBuffer;
};


class JDL_API VulkanVertexBuffer : public VulkanBuffer
{
public:
    /**
     * @brief Creates the Vulkan vertex buffer.
     * @param size Buffer size, in bytes.
     */
    VulkanVertexBuffer(uint64_t size);

    /**
     * @brief Binds the vertex buffer.
     * @param commandBuffer Command buffer with which the bind command will be recorded.
     */
    void bind(VkCommandBuffer commandBuffer);
};


class JDL_API VulkanIndexBuffer : public VulkanBuffer
{
public:
    /**
     * @brief Creates the Vulkan index buffer.
     * @param size Buffer size, in bytes.
     */
    VulkanIndexBuffer(uint64_t size);

    /**
     * @brief Binds the index buffer.
     * @param commandBuffer Command buffer with which the bind command will be recorded.
     */
    void bind(VkCommandBuffer commandBuffer);
};

} // namespace vk
} // namespace jdl
