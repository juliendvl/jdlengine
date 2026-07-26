#include "resource/shader.hpp"

#include <fstream>

#include "utils/logger.hpp"

#include "vk/vulkan_context.hpp"


namespace jdl
{
namespace resource
{

Shader::Shader(const std::string& name, const std::string& path)
	: Resource(name)
	, m_path(path)
{
	m_device = vk::VulkanContext::GetDevice().get_device();
	create_shader();
}

void Shader::reload()
{
	clear_resource();
	create_shader();
}

void Shader::create_shader()
{
	// Read the shader file
	std::ifstream stream(m_path, std::ios::binary | std::ios::ate);
	if (!stream)
	{
		JDL_ERROR("Failed to read shader {}", m_path);
		return;
	}

	size_t code_size = stream.tellg();
	std::vector<char> code(code_size);

	stream.seekg(0);
	stream.read(code.data(), code_size);

	VkShaderModuleCreateInfo create_info {};
	create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	create_info.codeSize = code_size;
	create_info.pCode = reinterpret_cast<const uint32_t*>(code.data());

	VK_CALL(vkCreateShaderModule(m_device, &create_info, nullptr, &m_module));
}

void Shader::clear_resource()
{
	if (m_module != VK_NULL_HANDLE)
	{
		vkDestroyShaderModule(m_device, m_module, nullptr);
		m_module = VK_NULL_HANDLE;
	}
}

} // namespace resource
} // namespace jdl
