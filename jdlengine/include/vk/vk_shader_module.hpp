#pragma once

#include <span>

#include "utils/non_copyable.hpp"

#include "vk_common.hpp"
#include "vk_device.hpp"


namespace jdl
{
namespace vk
{

class ShaderModule : private NonCopyable<ShaderModule>
{
public:
	/**
	 * @brief Creates a shader module from SPIR-V bytecode.
	 * @param device The device the shader module is created for.
	 * @param code The SPIR-V bytecode, as 32-bit words. It is only read during
	 * construction and does not need to outlive the shader module.
	 */
	ShaderModule(const Device& device, std::span<const uint32_t> code);

	/**
	 * @brief Destroys the shader module.
	 */
	~ShaderModule();

	/**
	 * @brief Returns the shader module handle.
	 */
	VkShaderModule handle() const { return m_shaderModule; }

private:
	VK_ATTR(VkDevice, m_device);
	VK_ATTR(VkShaderModule, m_shaderModule);
};

} // namespace vk
} // namespace jdl
