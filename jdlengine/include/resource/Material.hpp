#pragma once

#include "Resource.hpp"


namespace jdl
{
namespace resource
{

class ShaderProgram;
class Texture;

class JDL_API Material : public Resource
{
public:
    /**
     * @brief Creates the material.
     * @param name Material name
     */
    Material(const std::string& name);

    /**
     * @brief Returns the base color factor.
     */
    const glm::vec4& getBaseColorFactor() const { return m_baseColorFactor; }

    /**
     * @brief Returns the base color texture.
     */
    resource::Texture* getBaseColorTexture() const { return m_baseColorTexture; }

    /**
     * @brief Sets the base color factor.
     */
    void setBaseColorFactor(const glm::vec4& color) { m_baseColorFactor = color; }

    /**
     * @brief Sets the base color texture.
     * @param texture The new texture. If nullptr, a default white texture will be used as placeholder
     */
    void setBaseColorTexture(Texture* texture);

    /**
     * @brief Binds the material data to the input shader program.
     * @param program Source shader program (must be active)
     */
    void bind(ShaderProgram* program);

private:
    // Base Color
    glm::vec4 m_baseColorFactor;
    Texture*  m_baseColorTexture;
};

} // namespace resource
} // namespace jdl
