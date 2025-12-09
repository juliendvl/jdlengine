#pragma once

#include "Resource.hpp"


namespace jdl
{
namespace resource
{

class JDL_API Shader : public Resource
{
public:
	/**
	 * @brief Creates the shader.
	 * @param name Shader name.
	 * @param path Shader path.
	 */
	Shader(const std::string& name, const std::string& path);

	/**
	 * @brief Returns the shader path.
	 */
	const std::string& getPath() const {
		return m_path;
	}

	/**
	 * @brief Returns the shader Vulkan module.
	 */
	VkShaderModule getModule() const {
		return m_module;
	}

	/**
	 * @brief Reloads the shader.
	 */
	void reload();

private:
	std::string	m_path;

	VkDevice m_device = VK_NULL_HANDLE;
	VkShaderModule m_module = VK_NULL_HANDLE;

	void createShader();
	void clearResource();
};

} // namespace resource
} // namespace jdl
