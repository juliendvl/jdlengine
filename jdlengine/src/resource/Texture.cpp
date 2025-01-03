#include "resource/Texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "utils/Logger.hpp"


namespace jdl
{
namespace resource
{

static const std::vector<GLenum> kInternalFormat {GL_RED, GL_RG, GL_RGB, GL_RGBA};
static const std::vector<GLenum> kFormat {GL_RED, GL_RG, GL_RGB, GL_RGBA};

Texture::Texture(const std::string& name, const std::string& imagePath)
    : Resource(name)
    , m_textureID(0)
    , m_width(0)
    , m_height(0)
    , m_nbChannels(0)
    , m_textureUnit(0)
{
    createTexture();

    // Load the image
    stbi_set_flip_vertically_on_load(true);

    unsigned char* texels = stbi_load(imagePath.c_str(), &m_width, &m_height, &m_nbChannels, 0);
    if (texels == nullptr)
    {
        JDL_ERROR("Failed to load image {}", imagePath);
        return;
    }

    glBindTexture(GL_TEXTURE_2D, m_textureID);
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        GLenum internalFormat = kInternalFormat[m_nbChannels - 1];
        GLenum format = kFormat[m_nbChannels - 1];

        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, texels);
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(texels);
}

Texture::Texture(const std::string& name, int width, int height, int nbChannels, const unsigned char* texels)
    : Resource(name)
    , m_textureID(0)
    , m_width(width)
    , m_height(height)
    , m_nbChannels(nbChannels)
    , m_textureUnit(0)
{
    createTexture();

    glBindTexture(GL_TEXTURE_2D, m_textureID);
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        GLenum internalFormat = kInternalFormat[m_nbChannels - 1];
        GLenum format = kFormat[m_nbChannels - 1];

        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, texels);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::setWrappingParameter(WrappingParameter param, WrappingValue value)
{
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glTexParameteri(GL_TEXTURE_2D, static_cast<GLenum>(param), static_cast<GLenum>(value));
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::setBorderColor(float r, float g, float b, float a)
{
    float borderColor[] = {r, g, b, a};

    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::setFilteringParameter(FilteringParameter param, FilteringValue value)
{
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glTexParameteri(GL_TEXTURE_2D, static_cast<GLenum>(param), static_cast<GLenum>(value));
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bind(uint32_t textureUnit)
{
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, m_textureID);

    m_textureUnit = textureUnit;
}

void Texture::unbind()
{
    glActiveTexture(GL_TEXTURE0 + m_textureUnit);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::createTexture()
{
    glGenTextures(1, &m_textureID);
    if (m_textureID == 0)
    {
        JDL_FATAL("Failed to create the OpenGL texture object");
    }
}

void Texture::clearResource()
{
    glDeleteTextures(1, &m_textureID);
    m_textureID = 0;
}

} // namespace resource
} // namespace jdl
