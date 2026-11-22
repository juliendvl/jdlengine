#include "vk/vk_shader_module.hpp"

#include <cassert>


namespace jdl
{
namespace vk
{

// First word of any valid SPIR-V module
static constexpr uint32_t SPIRV_MAGIC_NUMBER = 0x07230203;

ShaderModule::ShaderModule(const Device& device, std::span<const uint32_t> code)
	: m_device(device.device())
{
	assert(!code.empty() && "SPIR-V code must not be empty");
	assert(code[0] == SPIRV_MAGIC_NUMBER && "Invalid SPIR-V magic number");

	VkShaderModuleCreateInfo create_info {
		.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
		.codeSize = code.size_bytes(),
		.pCode = code.data()
	};
	VK_CHECK(vkCreateShaderModule(m_device, &create_info, nullptr, &m_shaderModule));
}

ShaderModule::~ShaderModule()
{
	vkDestroyShaderModule(m_device, m_shaderModule, nullptr);
}

} // namespace vk
} // namespace jdl
