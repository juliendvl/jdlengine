#pragma once

#include "utils/NonCopyable.hpp"

#include "resource/Shader.hpp"


namespace jdl
{
namespace vk
{

class VulkanPipeline : private NonCopyable<VulkanPipeline>
{
public:
	VulkanPipeline();
	~VulkanPipeline();

	/**
	 * @brief Sets the vertex/fragment shader.
	 * @param shader The new shader.
	 */
	void setShader(resource::Shader* shader);

	/**
	 * @brief Creates the pipeline.
	 */
	void create();

	/**
	 * @brief Returns the pipeline layout handle.
	 */
	VkPipelineLayout getPipelineLayout() const { return m_pipelineLayout; }

	/**
	 * @brief Returns the pipeline handle.
	 */
	VkPipeline getPipeline() const { return m_pipeline; }

private:
	VK_ATTR(VkDevice, m_device);
	VK_ATTR(VkPipeline, m_pipeline);
	VK_ATTR(VkPipelineLayout, m_pipelineLayout);

	// Vertex/Fragment shader
	resource::Shader* m_shader = nullptr;

	void createPipelineLayout();
	void createPipeline();
	void destroy();
};

} // namespace vk
} // namespace jdl
