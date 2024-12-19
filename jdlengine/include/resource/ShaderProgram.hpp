#pragma once

#include "Resource.hpp"


namespace jdl
{
namespace resource
{

enum class ShaderType
{
    kVertex = GL_VERTEX_SHADER,
    kFragment = GL_FRAGMENT_SHADER
};


class JDL_API ShaderProgram : public Resource
{
public:
    /**
     * @brief Creates an empty shader program.
     * @param name Program name
     */
    ShaderProgram(const std::string& name);

    /**
     * @brief Creates a shader program with vertex and fragment shaders.
     * @param name  Program name
     * @param vPath Vertex shader path
     * @param fPath Fragment shader path
     */
    ShaderProgram(const std::string& name, const std::string& vPath, const std::string& fPath);

    /**
     * @brief  Adds a new shader.
     * @param  type Shader type
     * @param  path Shader file path
     * @return Whether the shader compilation has succeeded or not
     */
    bool addShader(ShaderType type, const std::string& path);

    /**
     * @brief  Links the shader program.
     * @return Whether the program link has succeeded or not
     */
    bool linkProgram();

    /**
     * @brief Reloads the shader program.
     */
    void reload();

    /**
     * @brief Starts using the shader program.
     */
    void use() const { glUseProgram(m_programID); }

    /**
     * @brief Stops using any shader program.
     */
    static void Stop() { glUseProgram(0); }

    /**
     * @brief  Returns the location of the input uniform variable.
     * @param  name Uniform name
     * @return Uniform location, or -1 if it does not exist
     */
    int getUniformLocation(const std::string& name);

    /**
     * @brief Sets the value of the input uniform variable.
     * @param name  Uniform name
     * @param value Uniform value
     */
    void setUniform(const std::string& name, bool value);
    void setUniform(const std::string& name, int value);
    void setUniform(const std::string& name, size_t value);
    void setUniform(const std::string& name, uint32_t value);
    void setUniform(const std::string& name, float value);
    void setUniform(const std::string& name, const glm::vec2& value);
    void setUniform(const std::string& name, const glm::vec3& value);
    void setUniform(const std::string& name, const glm::vec4& value);
    void setUniform(const std::string& name, const glm::mat3& value);
    void setUniform(const std::string& name, const glm::mat4& value);

private:
    // Program shaders
    using Shader = std::pair<std::string, GLuint>;
    std::unordered_map<ShaderType, Shader> m_shaders;

    // OpenGL program
    GLuint m_programID;

    // Uniform variables
    std::unordered_map<std::string, int> m_uniforms;

    // Creates the OpenGL shader program
    void createProgram();
    // Deletes the program and its shaders
    void clearResource();
};

} // namespace resource
} // namespace jdl
