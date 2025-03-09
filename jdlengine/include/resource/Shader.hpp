#pragma once

#include "Resource.hpp"


namespace jdl
{
namespace resource
{

class JDL_API Shader : public Resource
{
public:
    /**
     * @brief Creates the shader.
     * @param name Shader name
     * @param path Shader file path
     */
    Shader(const std::string& name, const std::string& path);

    /**
     * @brief Returns the shader SPV file path.
     */
    const std::string& getPath() const { return m_filePath; }

    /**
     * @brief Returns the shader Vulkan module.
     */
    VkShaderModule getModule() const { return m_module; }

    /**
     * @brief Reloads the shader.
     */
    void reload() {
        clearResource();
        createShader();
    }

private:
    // Shader SPV file path
    std::string m_filePath;
    // Vulkan module
    VkShaderModule m_module;

    // Reads the shader and creates the Vulkan module
    void createShader();
    // Clears the shader
    void clearResource();
};

} // namespace resource
} // namespace jdl
