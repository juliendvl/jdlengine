#include "resource/ShaderProgram.hpp"

#include <fstream>
#include <sstream>

#include <glm/gtc/type_ptr.hpp>

#include "utils/Logger.hpp"


namespace jdl
{
namespace resource
{

static std::string s_ReadFile(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        JDL_ERROR("Failed to open the shader file {}", path);
        return "";
    }

    std::stringstream stream;
    stream << file.rdbuf();

    return stream.str();
}


ShaderProgram::ShaderProgram(const std::string& name)
    : Resource(name)
    , m_pid(createProgram())
{}

ShaderProgram::ShaderProgram(const std::string& name, const std::string& vertexFile, const std::string& fragmentFile)
    : Resource(name)
    , m_pid(createProgram())
{
    addShader(ShaderType::kVertex, vertexFile);
    addShader(ShaderType::kFragment, fragmentFile);

    link();
}

bool ShaderProgram::addShader(ShaderType type, const std::string& path)
{
    // Read the shader content
    std::string source = s_ReadFile(path);
    if (source.empty())
    {
        return false;
    }
    const GLchar* src = static_cast<const GLchar*>(source.c_str());
    
    // Create the shader (if necessary) and compile it
    GLuint shaderID = 0;

    auto it = m_shaders.find(type);
    if (it == m_shaders.end())
    {
        shaderID = glCreateShader(static_cast<GLenum>(type));
        if (shaderID == 0)
        {
            JDL_FATAL("Failed to create the OpenGL shader object!");
        }
        m_shaders[type] = Shader(shaderID, path);
    }
    else
    {
        shaderID = it->second.id;
    }

    glShaderSource(shaderID, 1, &src, nullptr);
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

        JDL_ERROR("Failed to compile shader {}:\n{}", path, log.data());

        return false;
    }

    return true;
}

bool ShaderProgram::link()
{
    if (m_shaders.find(ShaderType::kVertex) == m_shaders.end())
    {
        JDL_ERROR("Cannot link the shader program '{}': missing vertex shader", getName());
        return false;
    }
    if (m_shaders.find(ShaderType::kFragment) == m_shaders.end())
    {
        JDL_ERROR("Cannot link the shader program '{}': missing fragment shader", getName());
        return false;
    }

    // Attach shaders to program and link
    for (const auto& [type, shader] : m_shaders)
    {
        glAttachShader(m_pid, shader.id);
    }
    glLinkProgram(m_pid);

    // Check link status
    GLint status;
    glGetProgramiv(m_pid, GL_LINK_STATUS, &status);

    if (status == GL_FALSE)
    {
        GLint logLength;
        glGetProgramiv(m_pid, GL_INFO_LOG_LENGTH, &logLength);

        std::vector<GLchar> log(logLength + 1);
        glGetProgramInfoLog(m_pid, logLength, &logLength, log.data());

        JDL_ERROR("Failed to link the shader program '{}':\n{}", getName(), log.data());
    }

    // The shaders can now be detached
    for (const auto& [type, shader] : m_shaders)
    {
        glDetachShader(m_pid, shader.id);
    }

    return status != GL_FALSE;
}

bool ShaderProgram::reload()
{
    m_uniforms.clear();

    for (const auto& [type, shader] : m_shaders)
    {
        addShader(type, shader.path);
    }

    return link();
}

int ShaderProgram::getUniformLocation(const std::string& name)
{
    auto it = m_uniforms.find(name);
    if (it == m_uniforms.end())
    {
        GLint location = glGetUniformLocation(m_pid, name.c_str());
        m_uniforms[name] = location;

        return location;
    }

    return it->second;
}

void ShaderProgram::setUniform(const std::string& name, int value)
{
    glUniform1i(getUniformLocation(name), value);
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

GLuint ShaderProgram::createProgram()
{
    GLuint pid = glCreateProgram();
    if (pid == 0)
    {
        JDL_FATAL("Failed to create the OpenGL shader program!");
    }
    return pid;
}

void ShaderProgram::clearResource()
{
    m_uniforms.clear();

    for (const auto& [shaderType, shader] : m_shaders)
    {
        glDeleteShader(shader.id);
    }
    m_shaders.clear();

    glDeleteProgram(m_pid);
}

} // namespace resource
} // namespace jdl
