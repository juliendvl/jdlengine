#pragma once

#include "Resource.hpp"

#include <unordered_map>


namespace jdl
{
namespace resource
{

enum class ShaderType
{
    kVertex   = GL_VERTEX_SHADER,
    kFragment = GL_FRAGMENT_SHADER
};


class JDL_API ShaderProgram : public Resource
{
public:
    /**
     * @brief Creates an empty shader program.
     * @param name Shader program name.
     */
    ShaderProgram(const std::string& name);

    /**
     * @brief Creates a shader program with vertex and fragment shaders.
     * @param name         Shader program name
     * @param vertexFile   Vertex shader path
     * @param fragmentFile Fragment shader path
     */
    ShaderProgram(const std::string& name, const std::string& vertexFile, const std::string& fragmentFile);

    /**
     * @brief Adds a new shader.
     * @param type Shader type
     * @param path Shader file path
     * @return Shader compilation status
     */
    bool addShader(ShaderType type, const std::string& path);

    /**
     * @brief Links the shader program.
     * @return Link status
     */
    bool link();

    /**
     * @brief Reloads the shader program.
     * @return Reload status
     */
    bool reload();

    /**
     * @brief Starts using this shader program.
     */
    void use() {
        glUseProgram(m_pid);
    }

    /**
     * @brief Resets the used shader program to 0.
     */
    static void Stop() {
        glUseProgram(0);
    }

    /**
     * @brief Returns the location of the input uniform variable.
     * @param name Uniform name
     * @return Uniform location, or -1 if it does not exist.
     */
    int getUniformLocation(const std::string& name);

    /**
     * @brief Sets the value of the input uniform variable.
     * @param name  Uniform name
     * @param value Uniform value
     */
    void setUniform(const std::string& name, bool value) { setUniform(name, value ? 1 : 0); }
    void setUniform(const std::string& name, int value);
    void setUniform(const std::string& name, float value);
    void setUniform(const std::string& name, const glm::vec2& value);
    void setUniform(const std::string& name, const glm::vec3& value);
    void setUniform(const std::string& name, const glm::vec4& value);
    void setUniform(const std::string& name, const glm::mat3& value);
    void setUniform(const std::string& name, const glm::mat4& value);

private:
    struct Shader
    {
        GLuint      id = 0;
        std::string path;

        Shader() = default;
        Shader(GLuint id, const std::string& path)
            : id(id)
            , path(path)
        {}
    };

    // Shaders map
    std::unordered_map<ShaderType, Shader> m_shaders;
    // Program OpenGL ID
    GLuint m_pid;
    // Uniforms map
    std::unordered_map<std::string, GLint> m_uniforms;

    // Creates and returns the OpenGL shader program object
    GLuint createProgram();
    // Clears the shader program data
    void clearResource();
};

} // namespace resource
} // namespace jdl
