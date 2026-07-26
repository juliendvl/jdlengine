#pragma once

#include "resource.hpp"


namespace jdl
{
namespace resource
{

class Shader : public Resource
{
public:
	/**
	 * @brief Creates the shader.
	 * @param name Shader name
	 * @param path Shader path (SPIR-V format)
	 */
	Shader(const std::string& name, const std::string& path);
	
	/**
	 * @brief Returns the shader path.
	 */
	const std::string& get_path() const { return m_path; }

	/**
	 * @brief Returns the shader Vulkan module.
	 */
	VkShaderModule get_module() const { return m_module; }
	
	/**
	 * @brief Returns whether the shader is valid (created module) or not.
	 */
	bool is_valid() const { return m_module != VK_NULL_HANDLE; }

	/**
	 * @brief Reloads the shader.
	 */
	void reload();

private:
	VK_ATTR(VkDevice, m_device);
	VK_ATTR(VkShaderModule, m_module);

	std::string m_path;

	void create_shader();
	void clear_resource() final;
};

} // namespace resource
} // namespace jdl
