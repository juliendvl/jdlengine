#include "resource/ShaderProgram.hpp"

#include <fstream>
#include <sstream>

#include <glm/gtc/type_ptr.hpp>

#include "utils/Logger.hpp"


namespace jdl
{
namespace resource
{

ShaderProgram::ShaderProgram(const std::string& name) : Resource(name), m_programID(0)
{
    createProgram();
}

ShaderProgram::ShaderProgram(const std::string& name, const std::string& vPath, const std::string& fPath)
    : Resource(name)
    , m_programID(0)
{
    createProgram();

    addShader(ShaderType::kVertex, vPath);
    addShader(ShaderType::kFragment, fPath);

    linkProgram();
}

bool ShaderProgram::addShader(ShaderType type, const std::string& path)
{
    // Retrieve the shader source code
    std::stringstream stream;
    std::ifstream file(path);
    if (!file)
    {
        JDL_ERROR("Failed to read the shader file {}", path);
        return false;
    }
    stream << file.rdbuf();    

    // Create the shader and compile it
    GLuint shaderID = glCreateShader(static_cast<GLenum>(type));
    if (shaderID == 0)
    {
        JDL_FATAL("Failed to create the OpenGL shader");
    }

    std::string shaderSource = stream.str();
    const char* source = shaderSource.c_str();

    glShaderSource(shaderID, 1, &source, nullptr);
    glCompileShader(shaderID);

    // Check the compilation status
    GLint status;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint logLength;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);

        std::vector<GLchar> log(logLength + 1);
        glGetShaderInfoLog(shaderID, logLength, &logLength, log.data());
        JDL_ERROR("Failed to compile shader {}: {}", path, log.data());

        glDeleteShader(shaderID);
        return false;
    }

    // If there is already a shader with the same type, delete it
    auto it = m_shaders.find(type);
    if (it != m_shaders.end())
    {
        glDeleteShader(it->second.second);
    }
    m_shaders[type] = std::make_pair(path, shaderID);

    return true;
}

bool ShaderProgram::linkProgram()
{
    if (m_shaders.find(ShaderType::kVertex) == m_shaders.end())
    {
        JDL_ERROR("Cannot link program {} - Missing Vertex shader", getName());
        return false;
    }
    if (m_shaders.find(ShaderType::kFragment) == m_shaders.end())
    {
        JDL_ERROR("Cannot link program {} - Missing Fragment shader", getName());
        return false;
    }

    // Attach the shaders and link
    for (const auto& [type, shader] : m_shaders)
    {
        glAttachShader(m_programID, shader.second);
    }
    glLinkProgram(m_programID);

    // Check the link status
    GLint status;
    glGetProgramiv(m_programID, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint logLength;
        glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &logLength);

        std::vector<GLchar> log(logLength + 1);
        glGetProgramInfoLog(m_programID, logLength, &logLength, log.data());

        JDL_ERROR("Failed to link program {}: {}", getName(), log.data());
    }

    // The shaders can now be detached
    for (const auto& [type, shader] : m_shaders)
    {
        glDetachShader(m_programID, shader.second);
    }

    return status == GL_TRUE;
}

void ShaderProgram::reload()
{
    for (const auto& [type, shader] : m_shaders)
    {
        addShader(type, shader.first);
    }
    linkProgram();

    m_uniforms.clear();
}

int ShaderProgram::getUniformLocation(const std::string& name)
{
    auto it = m_uniforms.find(name);
    if (it != m_uniforms.end())
    {
        return it->second;
    }

    int location = glGetUniformLocation(m_programID, name.c_str());
    m_uniforms[name] = location;

    return location;
}

void ShaderProgram::setUniform(const std::string& name, bool value)
{
    glUniform1i(getUniformLocation(name), value ? 1 : 0);
}

void ShaderProgram::setUniform(const std::string& name, int value)
{
    glUniform1i(getUniformLocation(name), value);
}

void ShaderProgram::setUniform(const std::string& name, size_t value)
{
    glUniform1ui(getUniformLocation(name), static_cast<GLuint>(value));
}

void ShaderProgram::setUniform(const std::string& name, uint32_t value)
{
    glUniform1ui(getUniformLocation(name), value);
}

void ShaderProgram::setUniform(const std::string& name, float value)
{
    glUniform1f(getUniformLocation(name), value);
}

void ShaderProgram::setUniform(const std::string& name, const glm::vec2& value)
{
    glUniform2fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

void ShaderProgram::setUniform(const std::string& name, const glm::vec3& value)
{
    glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

void ShaderProgram::setUniform(const std::string& name, const glm::vec4& value)
{
    glUniform4fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

void ShaderProgram::setUniform(const std::string& name, const glm::mat3& value)
{
    glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

void ShaderProgram::setUniform(const std::string& name, const glm::mat4& value)
{
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

void ShaderProgram::createProgram()
{
    m_programID = glCreateProgram();
    if (m_programID == 0)
    {
        JDL_FATAL("Failed to create the OpenGL shader program");
    } 
}

void ShaderProgram::clearResource()
{
    for (const auto& [type, shader] : m_shaders)
    {
        glDeleteShader(shader.second);
    }
    glDeleteProgram(m_programID);
}

} // namespace resource
} // namespace jdl
