#pragma once

#include "Core.hpp"

#include "utils/NonCopyable.hpp"


namespace jdl
{
namespace core
{

enum BufferUsage
{
    kTransferSrc = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
    kTransferDst = VK_BUFFER_USAGE_TRANSFER_DST_BIT,
    kIndex       = VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
    kVertex      = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT
};

inline BufferUsage  operator&(BufferUsage a, BufferUsage b) { return (BufferUsage)((int)a & (int)b); }
inline BufferUsage  operator|(BufferUsage a, BufferUsage b) { return (BufferUsage)((int)a | (int)b); }

inline BufferUsage& operator&=(BufferUsage& a, BufferUsage b) { return (BufferUsage&)((int&)a &= (int)b); }
inline BufferUsage& operator|=(BufferUsage& a, BufferUsage b) { return (BufferUsage&)((int&)a |= (int)b); }


enum MemoryProperty
{
    kDeviceLocal  = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
    kHostVisible  = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
    kHostCoherent = VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
};

inline MemoryProperty  operator&(MemoryProperty a, MemoryProperty b) { return (MemoryProperty)((int)a & (int)b); }
inline MemoryProperty  operator|(MemoryProperty a, MemoryProperty b) { return (MemoryProperty)((int)a | (int)b); }

inline MemoryProperty& operator&=(MemoryProperty& a, MemoryProperty b) { return (MemoryProperty&)((int&)a &= (int)b); }
inline MemoryProperty& operator|=(MemoryProperty& a, MemoryProperty b) { return (MemoryProperty&)((int&)a |= (int)b); }


class JDL_API VulkanBuffer : private NonCopyable<VulkanBuffer>
{
public:
    /**
     * @brief Creates the buffer.
     * @param size             Buffer size, in bytes
     * @param usage            Buffer usage
     * @param memoryProperties Buffer memory properties
     */
    VulkanBuffer(uint64_t size, BufferUsage usage, MemoryProperty memoryProperties);

    /**
     * @brief Destroys the buffer
     */
    ~VulkanBuffer();

    /**
     * @brief Returns the buffer size, in bytes.
     */
    uint64_t getSize() const { return m_size; }

    /**
     * @brief Returns the buffer usage(s).
     */
    BufferUsage getUsage() const { return m_usage; }

    /**
     * @brief Returns the buffer memory properties.
     */
    MemoryProperty getMemoryProperties() const { return m_memoryProperties; }

    /**
     * @brief Returns the Vulkan buffer object.
     */
    VkBuffer getBuffer() const { return m_buffer; }

    /**
     * @brief Sets the buffer data.
     * @param data   Data to be uploaded
     * @param size   Data size in bytes. Set to default to update the whole buffer.
     * @param offset Offset in bytes where the data should be uploaded. Set to default to upload the data at the start.
     */
    void setData(void* data, uint64_t size = 0, uint64_t offset = 0);

private:
    uint64_t m_size;
    BufferUsage m_usage;
    MemoryProperty m_memoryProperties;

    VkBuffer m_buffer;
    VkDeviceMemory m_deviceMemory;

    void createBuffer(VkDevice device);
    void allocateMemory(VkDevice device);
};


class JDL_API Buffer : private NonCopyable<Buffer>
{
public:
    /**
     * @brief Creates the buffer.
     * @param size  Buffer size, in bytes
     * @param usage Buffer usage
     */
    Buffer(uint64_t size, BufferUsage usage);

    /**
     * @brief Returns the buffer size, in bytes.
     */
    uint64_t getSize() const { return m_stagingBuffer.getSize(); }

    /**
     * @brief Returns the buffer usage(s).
     */
    BufferUsage getUsage() const { return m_usage; }

    /**
     * @brief Returns the internal staging buffer.
     */
    const VulkanBuffer& getStagingBuffer() const { return m_stagingBuffer; }

    /**
     * @brief Returns the internal device buffer.
     */
    const VulkanBuffer& getDeviceBuffer() const { return m_deviceBuffer; }

    /**
     * @brief Sets the buffer data.
     * @param data   Data to be uploaded
     * @param size   Data size in bytes. Set to default to update the whole buffer.
     * @param offset Offset in bytes where the data should be uploaded. Set to default to upload the data at the start.
     */
    void setData(void* data, uint64_t size = 0, uint64_t offset = 0);

    /**
     * @brief Binds the buffer.
     * @param commandBuffer Command buffer on which the bind command will be recorded
     */
    virtual void bind(VkCommandBuffer commandBuffer) {}

private:
    BufferUsage m_usage;

    VulkanBuffer m_stagingBuffer;
    VulkanBuffer m_deviceBuffer;
};


class JDL_API VertexBuffer : public Buffer
{
public:
    /**
     * @brief Creates the buffer.
     * @param size Buffer size, in bytes
     */
    VertexBuffer(uint64_t size);

    /**
     * @brief Binds the buffer.
     * @param commandBuffer Command buffer on which the bind command will be recorded
     */
    void bind(VkCommandBuffer commandBuffer);
};


class JDL_API IndexBuffer : public Buffer
{
public:
    /**
     * @brief Creates the buffer.
     * @param size Buffer size, in bytes
     */
    IndexBuffer(uint64_t size);

    /**
     * @brief Binds the buffer.
     * @param commandBuffer Command buffer on which the bind command will be recorded
     */
    void bind(VkCommandBuffer commandBuffer);
};

} // namespace core
} // namespace jdl
