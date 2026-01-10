#pragma once

#include "Resource.hpp"

#include "vulkan/VulkanBuffer.hpp"
#include "vulkan/VulkanImage.hpp"

#include <unordered_map>


namespace jdl
{
namespace resource
{

enum class FilteringParameter
{
    eMagFilter,
    eMinFilter
};

enum class FilteringValue
{
    eNearest = VK_FILTER_NEAREST,
    eLinear = VK_FILTER_LINEAR
};

enum class WrappingParameter
{
    eWrapU,
    eWrapV,
    eWrapW
};

enum class WrappingValue
{
    eRepeat = VK_SAMPLER_ADDRESS_MODE_REPEAT,
    eMirroredRepeat = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT,
    eClampToEdge = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
    eMirrorClampToEdge = VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE,
    eClampToBorder = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER
};


class JDL_API Texture : public Resource
{
public:
    /**
     * @brief Inits the texture.
     * @param name Texture name.
     */
    Texture(const std::string& name);

    /**
     * @brief Sets the texture filtering parameter.
     * @param parameter Filtering parameter.
     * @param value     Filtering value.
     */
    void setFilteringParameter(FilteringParameter parameter, FilteringValue value) {
        m_filtering[parameter] = value;
    }

    /**
     * @brief Sets the texture wrapping parameter.
     * @param parameter Wrapping parameter.
     * @param value     Wrapping value.
     */
    void setWrappingParameter(WrappingParameter parameter, WrappingValue value) {
        m_wrapping[parameter] = value;
    }

    /**
     * @brief Creates the texture.
     * @param path Texture image path.
     */
    void create(const std::string& path);

    /**
     * @brief Creates the texture.
     * @param width     Texture width.
     * @param height    Texture height.
     * @param texels    Texture texels.
     */
    void create(int width, int height, const unsigned char* texels);

    /**
     * @brief Returns the texture width.
     */
    int getWidth() const { return m_width; }

    /**
     * @brief Returns the texture height.
     */
    int getHeight() const { return m_height; }

    /**
     * @brief Returns the Vulkan image view.
     */
    VkImageView getImageView() const { return m_image->getView(); }

    /**
     * @brief Returns the Vulkan sampler.
     */
    VkSampler getSampler() const { return m_sampler; }

private:
    VkDevice m_device = VK_NULL_HANDLE;

    // Texture dimensions
    int m_width = -1;
    int m_height = -1;
    int m_nbChannels = 4;

    // Filtering/Wrapping
    std::unordered_map<FilteringParameter, FilteringValue> m_filtering {
        {FilteringParameter::eMagFilter, FilteringValue::eLinear},
        {FilteringParameter::eMinFilter, FilteringValue::eLinear}
    };
    std::unordered_map<WrappingParameter, WrappingValue> m_wrapping {
        {WrappingParameter::eWrapU, WrappingValue::eRepeat},
        {WrappingParameter::eWrapV, WrappingValue::eRepeat},
        {WrappingParameter::eWrapW, WrappingValue::eRepeat}
    };

    // Texture Vulkan objects
    std::unique_ptr<vk::VulkanImage> m_image;
    VkSampler m_sampler = VK_NULL_HANDLE;

    void doCreate(const unsigned char* texels);
    
    void createImage();
    void copyBufferToImage(const vk::VulkanBufferWrapper& buffer);
    void createSampler();

    void clearResource();
};

} // namespace resource
} // namespace jdl
