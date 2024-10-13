#pragma once

#include "Resource.hpp"


namespace jdl
{
namespace resource
{

enum class WrapParameter
{
    kWrapR = GL_TEXTURE_WRAP_R,
    kWrapS = GL_TEXTURE_WRAP_S,
    kWrapT = GL_TEXTURE_WRAP_T
};

enum class WrapValue
{
    kRepeat         = GL_REPEAT,
    kMirroredRepeat = GL_MIRRORED_REPEAT,
    kClampToEdge    = GL_CLAMP_TO_EDGE,
    kClampToBorder  = GL_CLAMP_TO_BORDER
};


enum class FilterParameter
{
    kMagnify = GL_TEXTURE_MAG_FILTER,
    kMinify  = GL_TEXTURE_MIN_FILTER
};

enum class FilterValue
{
    kLinear  = GL_LINEAR,
    kNearest = GL_NEAREST
};


class JDL_API Texture : public Resource
{
public:
    /**
     * @brief Creates a texture from an image.
     * @param name Texture name
     * @param path Texture file path
     */
    Texture(const std::string& name, const std::string& path);

    /**
     * @brief Creates a texture from texels.
     * @param name       Texture name
     * @param width      Texture width
     * @param height     Texture height
     * @param nbChannels Number of color channels
     * @param texels     Texture texels
     */
    Texture(
        const std::string& name,
        int width,
        int height,
        int nbChannels,
        const unsigned char* const texels
    );

    /**
     * @brief Returns the texture width.
     */
    int getWidth() const {
        return m_width;
    }

    /**
     * @brief Returns the texture height.
     */
    int getHeight() const {
        return m_height;
    }

    /**
     * @brief Returns the texture number of color channels.
     */
    int getNbChannels() const {
        return m_channels;
    }

    /**
     * @brief Sets the texture wrapping parameter.
     * @param parameter Wrapping parameter
     * @param value     Wrapping value
     */
    void setWrappingParameter(WrapParameter parameter, WrapValue value);

    /**
     * @brief When using the "clamp to border" wrapping parameter, sets the border color.
     * @param r, g, b, a RGB color components
     * @param color      Color vector
     */
    void setWrappingBorderColor(float r, float g, float b, float a = 1.0f);
    void setWrappingBorderColor(const glm::vec4& color);

    /**
     * @brief Sets the texture filtering parameter.
     * @param parameter Filtering parameter
     * @param value     Filtering value
     */
    void setFilteringParameter(FilterParameter parameter, FilterValue value);

    /**
     * @brief Binds the texture on the specified unit.
     */
    void bind(uint32_t unit = 0) const;

    /**
     * @brief Unbinds the texture.
     */
    void unbind() const {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

private:
    // Texture info (dimensions, number of channels)
    int m_width;
    int m_height;
    int m_channels;
    // OpenGL data
    GLuint m_textureID;

    void createTexture();
    void clearResource();
};

} // namespace resource
} // namespace jdl
