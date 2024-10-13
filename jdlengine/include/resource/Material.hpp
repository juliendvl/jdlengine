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
     * @brief Returns the albedo texture.
     */
    Texture* getAlbedoTexture() const {
        return m_albedoTexture;
    }

    /**
     * @brief Sets the albedo texture.
     */
    void setAlbedoTexture(Texture* albedo) {
        m_albedoTexture = albedo;
    }

    /**
     * @brief Binds the material textures and updates the current shader.
     * @param context Render context
     */
    void bind(const core::RenderContext& context);

private:
    Texture* m_albedoTexture;
};

} // namespace resource
} // namespace jdl
