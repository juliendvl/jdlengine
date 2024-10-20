#include "resource/Material.hpp"


namespace jdl
{
namespace resource
{

Material::Material(const std::string& name)
    : Resource(name)
    , m_baseColorFactor(1.0f)
    , m_baseColorTexture(nullptr)
{}

void Material::bind(const core::RenderContext& context)
{   
    context.shaderProgram->setUniform("material.baseColorFactor", m_baseColorFactor);
    if (m_baseColorTexture)
    {
        m_baseColorTexture->bind(0);
        context.shaderProgram->setUniform("material.baseColorTexture", 0);
    }
}

} // namespace resource
} // namespace jdl
