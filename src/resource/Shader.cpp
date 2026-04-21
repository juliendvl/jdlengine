#include "resource/Shader.hpp"

#include "utils/Logger.hpp"

#include "vk/VulkanContext.hpp"

#include <fstream>


namespace jdl
{
namespace resource
{

Shader::Shader(const std::string& name, const std::string& path)
	: Resource(name)
	, m_path(path)
{
	m_device = vk::VulkanContext::GetDevice().getDevice();
	createShader();
}

void Shader::reload()
{
	clearResource();
	createShader();
}

void Shader::createShader()
{
	std::ifstream stream(m_path, std::ios::binary | std::ios::ate);
	if (!stream)
	{
		JDL_ERROR("Failed to read shader {}", m_path);
		return;
	}

	size_t codeSize = stream.tellg();
	std::vector<char> code(codeSize);

	stream.seekg(0);
	stream.read(code.data(), codeSize);

	VkShaderModuleCreateInfo createInfo {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = codeSize;
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	VK_CALL(vkCreateShaderModule(m_device, &createInfo, nullptr, &m_module));
}

void Shader::clearResource()
{
	vkDestroyShaderModule(m_device, m_module, nullptr);
	m_module = VK_NULL_HANDLE;
}

} // namespace resource
} // namespace jdl
