#include "resource/Material.hpp"
#include "resource/ShaderProgram.hpp"
#include "resource/Texture.hpp"


namespace jdl
{
namespace resource
{

Material::Material(const std::string& name)
    : Resource(name)
    , m_baseColorFactor(1.0f)
    , m_baseColorTexture(nullptr)
{
    setBaseColorTexture(nullptr);
}

void Material::setBaseColorTexture(Texture* texture)
{
    if (texture == nullptr)
    {
        texture = ResourceManager::As<Texture>("__WHITE_TEXTURE__");
    }
    m_baseColorTexture = texture;
}

void Material::bind(ShaderProgram* program)
{
    program->setUniform("material.baseColorFactor", m_baseColorFactor);

    m_baseColorTexture->bind(0);
    program->setUniform("material.baseColorTexture", 0);
}


} // namespace resource
} // namespace jdl
