#include "resource/Texture.hpp"

#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "utils/Logger.hpp"


namespace jdl
{
namespace resource
{

static const std::array<GLuint, 4> kInternalFormats {
    GL_RED,
    GL_RG,
    GL_RGB,
    GL_RGBA,
};

static const std::array<GLuint, 4> kFormats {
    GL_RED,
    GL_RG,
    GL_RGB,
    GL_RGBA,
};


Texture::Texture(const std::string& name, const std::string& path)
    : Resource(name)
    , m_width(0)
    , m_height(0)
    , m_channels(0)
    , m_textureID(0)
{
    createTexture();

    // Read the texture
    unsigned char* texels = stbi_load(path.c_str(), &m_width, &m_height, &m_channels, 0);
    if (texels == 0)
    {
        JDL_ERROR("Failed to read texture {}", path);
        return;
    }

    glBindTexture(GL_TEXTURE_2D, m_textureID);
    {
        // Set the initial wrapping and filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Upload the texture
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            kInternalFormats[m_channels - 1],
            m_width,
            m_height,
            0,
            kFormats[m_channels - 1],
            GL_UNSIGNED_BYTE,
            texels
        );
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(const std::string& name, int width, int height, int nbChannels, const unsigned char* const texels)
    : Resource(name)
    , m_width(width)
    , m_height(height)
    , m_channels(nbChannels)
    , m_textureID(0)
{
    createTexture();

    glBindTexture(GL_TEXTURE_2D, m_textureID);
    {
        // Set the initial wrapping and filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Upload the texture
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            kInternalFormats[m_channels - 1],
            m_width,
            m_height,
            0,
            kFormats[m_channels - 1],
            GL_UNSIGNED_BYTE,
            texels
        );
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::setWrappingParameter(WrapParameter parameter, WrapValue value)
{
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glTexParameteri(GL_TEXTURE_2D, static_cast<GLenum>(parameter), static_cast<GLenum>(value));
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::setWrappingBorderColor(float r, float g, float b, float a)
{
    std::array<float, 4> borderColor{r, g, b, a};

    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor.data());
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::setWrappingBorderColor(const glm::vec4& color)
{
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(color));
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::setFilteringParameter(FilterParameter parameter, FilterValue value)
{
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glTexParameteri(GL_TEXTURE_2D, static_cast<GLenum>(parameter), static_cast<GLenum>(value));
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bind(uint32_t unit) const
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
}

void Texture::createTexture()
{
    glGenTextures(1, &m_textureID);
    if (m_textureID == 0)
    {
        JDL_FATAL("Failed to create the OpenGL texture!");
    }
}

void Texture::clearResource()
{
    glDeleteTextures(1, &m_textureID);
}

} // namespace resource
} // namespace jdl
