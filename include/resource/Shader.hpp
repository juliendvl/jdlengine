#pragma once

#include "Resource.hpp"


namespace jdl
{
namespace resource
{

class Shader : public Resource
{
public:
	/**
	 * @brief Creates the shader.
	 * @param name Shader name.
	 * @param path Shader file path (SPIR-V format).
	 */
	Shader(const std::string& name, const std::string& path);

	/**
	 * @brief Returns the Vulkan shader module.
	 */
	VkShaderModule getModule() const { return m_module; }

	/**
	 * @brief Reloads the shader.
	 */
	void reload();

private:
	VK_ATTR(VkDevice, m_device);
	VK_ATTR(VkShaderModule, m_module);

	std::string m_path;

	void createShader();
	void clearResource();
};

} // namespace resource
} // namespace jdl
