#include "resource/Material.hpp"


namespace jdl
{
namespace resource
{

Material::Material(const std::string& name)
    : Resource(name)
    , m_albedoTexture(nullptr)
{}

void Material::bind(const core::RenderContext& context)
{
    if (m_albedoTexture)
    {
        m_albedoTexture->bind(0);
        context.shaderProgram->setUniform("material.albedo", 0);
    }
}

} // namespace resource
} // namespace jdl
