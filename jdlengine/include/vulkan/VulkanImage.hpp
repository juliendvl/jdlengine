#pragma once

#include "core/Core.hpp"

#include "utils/NonCopyable.hpp"


namespace jdl
{
namespace vk
{

enum ImageUsage
{
    eImageTransferDst   = VK_IMAGE_USAGE_TRANSFER_DST_BIT,
    eSampled            = VK_IMAGE_USAGE_SAMPLED_BIT,
    eDepthStencil       = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT
};

inline ImageUsage operator&(ImageUsage a, ImageUsage b) { return (ImageUsage)((int)a & (int)b); }
inline ImageUsage operator|(ImageUsage a, ImageUsage b) { return (ImageUsage)((int)a | (int)b); }

inline ImageUsage& operator&=(ImageUsage& a, ImageUsage b)
{
    a = a & b;
    return a;
}

inline ImageUsage& operator|=(ImageUsage& a, ImageUsage b)
{
    a = a | b;
    return a;
}


enum class ImageTiling
{
    eLinear     = VK_IMAGE_TILING_LINEAR,
    eOptimal    = VK_IMAGE_TILING_OPTIMAL
};


class JDL_API VulkanImage : private NonCopyable<VulkanImage>
{
public:
    /**
     * @brief Creates the image.
     * @param width     Image width.
     * @param height    Image height.
     * @param usage     Image usage.
     * @param tiling    Image tiling (default = optimal).
     */
    VulkanImage(
        uint32_t width,
        uint32_t height,
        ImageUsage usage,
        ImageTiling tiling = ImageTiling::eOptimal
    );

    /**
     * @brief Destroys the image data.
     */
    ~VulkanImage();

    /**
     * @brief Returns the image format.
     */
    VkFormat getFormat() const { return m_format; }

    /**
     * @brief Returns the image Vulkan object.
     */
    VkImage get() const { return m_image; }

    /**
     * @brief Returns the image view Vulkan object.
     */
    VkImageView getView() const { return m_imageView; }

private:
    VkDevice m_device = VK_NULL_HANDLE;

    // Image format
    VkFormat m_format = VK_FORMAT_UNDEFINED;

    // Vulkan objects
    VkImage m_image = VK_NULL_HANDLE;
    VkDeviceMemory m_deviceMemory = VK_NULL_HANDLE;
    VkImageView m_imageView = VK_NULL_HANDLE;
};

} // namespace vk
} // namespace jdl
