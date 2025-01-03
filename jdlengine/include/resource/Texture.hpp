#pragma once

#include "Resource.hpp"


namespace jdl
{
namespace resource
{

enum class WrappingParameter
{
    kWrapR = GL_TEXTURE_WRAP_R,
    kWrapS = GL_TEXTURE_WRAP_S,
    kWrapT = GL_TEXTURE_WRAP_T,
};

enum class WrappingValue
{
    kClampToBorder  = GL_CLAMP_TO_BORDER,
    kClampToEdge    = GL_CLAMP_TO_EDGE,
    kMirroredRepeat = GL_MIRRORED_REPEAT,
    kRepeat         = GL_REPEAT
};


enum class FilteringParameter
{
    kMagnify = GL_TEXTURE_MAG_FILTER,
    kMinify  = GL_TEXTURE_MIN_FILTER
};

enum class FilteringValue
{
    kLinear  = GL_LINEAR,
    kNearest = GL_NEAREST
};


class JDL_API Texture : public Resource
{
public:
    /**
     * @brief Creates a texture from an input file.
     * @param name      Texture name
     * @param imagePath Texture image path
     */
    Texture(const std::string& name, const std::string& imagePath);

    /**
     * @brief Creates a texture from texels.
     * @param name       Texture name
     * @param width      Texture width
     * @param height     Texture height
     * @param nbChannels Number of color channels
     * @param texels     Texels data
     */
    Texture(const std::string& name, int width, int height, int nbChannels, const unsigned char* texels);

    /**
     * @brief Returns the texture width.
     */
    int getWidth() const { return m_width; }

    /**
     * @brief Returns the texture height.
     */
    int getHeight() const { return m_height; }

    /**
     * @brief Returns the texture number of channels.
     */
    int getNbChannels() const { return m_nbChannels; }

    /**
     * @brief Sets the texture wrapping parameter.
     * @param param Wrapping parameter
     * @param value Parameter value
     */
    void setWrappingParameter(WrappingParameter param, WrappingValue value);

    /**
     * @brief When using the "Clamp to Border" wrapping value, sets the border color.
     * @param r, g, b, a Border color
     */
    void setBorderColor(float r, float g, float b, float a = 1.0f);

    /**
     * @brief Sets the texture filtering parameter.
     * @param param Filtering parameter
     * @param value Parameter value
     */
    void setFilteringParameter(FilteringParameter param, FilteringValue value);

    /**
     * @brief Binds the texture.
     * @param textureUnit Texture unit on which the texture should be bound
     */
    void bind(uint32_t textureUnit = 0);

    /**
     * @brief Unbinds the texture.
     */
    void unbind();

private:
    // OpenGL ID
    GLuint m_textureID;
    // Texture metadata
    int m_width;
    int m_height;
    int m_nbChannels;
    // Texture unit (when bound)
    uint32_t m_textureUnit;

    // Creates the OpenGL texture object
    void createTexture();
    // Destroys the OpenGL texture object
    void clearResource();
};

} // namespace resource
} // namespace jdl
