#include "resource/Shader.hpp"

#include <fstream>

#include "core/VulkanContext.hpp"

#include "utils/Logger.hpp"


namespace jdl
{
namespace resource
{

Shader::Shader(const std::string& name, const std::string& path)
    : Resource(name)
    , m_filePath(path)
    , m_module(VK_NULL_HANDLE)
{
    createShader();
}

void Shader::createShader()
{
    // Read the shader
    std::ifstream stream(m_filePath, std::ios::binary | std::ios::ate);
    if (!stream)
    {
        JDL_ERROR("Failed to read shader {}", m_filePath);
        return;
    }

    size_t codeSize = stream.tellg();
    stream.seekg(0);

    std::vector<char> code(codeSize);
    stream.read(code.data(), codeSize);

    // Create the shader module
    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = codeSize;
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkDevice device = core::VulkanContext::GetDevice();
    VK_CALL(vkCreateShaderModule(device, &createInfo, nullptr, &m_module));
}

void Shader::clearResource()
{
    if (m_module != VK_NULL_HANDLE)
    {
        VkDevice device = core::VulkanContext::GetDevice();
        vkDestroyShaderModule(device, m_module, nullptr);

        m_module = VK_NULL_HANDLE;
    }
}

} // namespace resource
} // namespace jdl
