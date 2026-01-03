#pragma once

#include "Resource.hpp"

#include "core/RenderContext.hpp"

#include "vulkan/VulkanBuffer.hpp"

#include <glm/glm.hpp>


namespace jdl
{
namespace resource
{

class Texture;

class JDL_API Material : public Resource
{
public:
    /**
     * @brief Creates the material.
     * @param name Material name.
     */
    Material(const std::string& name);

    /**
     * @brief Returns the base color factor.
     */
    const glm::vec4& getBaseColorFactor() const { return m_baseColorFactor; }

    /**
     * @brief Sets the base color factor.
     * @param factor New value.
     */
    void setBaseColorFactor(const glm::vec4& factor);

    /**
     * @brief Returns the base color texture.
     */
    Texture* getBaseColorTexture() const { return m_baseColorTexture; }

    /**
     * @brief Sets the base color texture.
     * @param texture New texture.
     */
    void setBaseColorTexture(Texture* texture);

    /**
     * @brief Binds the material.
     * @param context Render context.
     */
    void bind(core::RenderContext& context);

private:
    struct MaterialUniformBuffer
    {
        alignas(16) glm::vec4 baseColorFactor;
    };

    // Notifies when the uniform buffer must be updated
    bool m_materialBufferDirty = true;
    // Notifies when the textures have been changed
    bool m_texturesDirty = true;

    // Base Color
    glm::vec4 m_baseColorFactor = glm::vec4(1.0f);
    Texture* m_baseColorTexture = nullptr;

    VkDevice m_device = VK_NULL_HANDLE;
    // Vulkan uniform buffer
    std::unique_ptr<vk::VulkanBufferWrapper> m_materialBuffer;
    // Vulkan descriptor set
    VkDescriptorSet m_descriptorSet = VK_NULL_HANDLE;

    void clearResource();
};

} // namespace resource
} // namespace jdl
