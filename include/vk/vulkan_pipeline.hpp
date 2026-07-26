#pragma once

#include "utils/non_copyable.hpp"

#include <unordered_map>


namespace jdl
{

namespace resource
{
class Shader;
} // namespace resource

namespace vk
{

enum class ShaderStage
{
	eVertex = VK_SHADER_STAGE_VERTEX_BIT,
	eFragment = VK_SHADER_STAGE_FRAGMENT_BIT
};

class VulkanPipeline : private NonCopyable<VulkanPipeline>
{
public:
	VulkanPipeline();
	~VulkanPipeline();

	/**
	 * @brief Adds a new shader for the pipeline configuration.
	 * This has to be called before creating the pipeline.
	 * 
	 * @param stage Pipeline shader stage
	 * @param shader Shader resource
	 */
	void add_shader(ShaderStage stage, resource::Shader* shader);

	/**
	 * @brief Creates the Vulkan pipeline.
	 */
	void create();

	/**
	 * @brief Returns the Vulkan pipeline layout handle.
	 */
	VkPipelineLayout get_pipeline_layout() const { return m_pipelineLayout; }

	/**
	 * @brief Returns the Vulkan pipeline handle.
	 */
	VkPipeline get_pipeline() const { return m_pipeline;  }

private:
	VK_ATTR(VkDevice, m_device);
	VK_ATTR(VkPipelineLayout, m_pipelineLayout);
	VK_ATTR(VkPipeline, m_pipeline);

	std::unordered_map<ShaderStage, resource::Shader*> m_shaders;

	void create_pipeline_layout();
	void create_pipeline();
};

} // namespace vk
} // namespace jdl
