#pragma once

#include "Resource.hpp"
#include "Texture.hpp"

#include "core/RenderContext.hpp"


namespace jdl
{
namespace resource
{

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
    const glm::vec4& getBaseColorFactor() const {
        return m_baseColorFactor;
    }

    /**
     * @brief Sets the base color factor.
     */
    void setBaseColorFactor(const glm::vec4& factor) {
        m_baseColorFactor = factor;
    }

    /**
     * @brief Returns the base color texture.
     */
    Texture* getBaseColorTexture() const {
        return m_baseColorTexture;
    }

    /**
     * @brief Sets the base color texture.
     */
    void setBaseColorTexture(Texture* texture) {
        m_baseColorTexture = texture;
    }

    /**
     * @brief Binds the material textures and updates the current shader.
     * @param context Render context
     */
    void bind(const core::RenderContext& context);

private:
    glm::vec4 m_baseColorFactor;
    Texture*  m_baseColorTexture;
};

} // namespace resource
} // namespace jdl
