#pragma once

#include "core/Core.hpp"

#include "resource/Shader.hpp"

#include "utils/NonCopyable.hpp"

#include <unordered_map>


namespace jdl
{
namespace vk
{

enum class ShaderStage
{
	eVertex = VK_SHADER_STAGE_VERTEX_BIT,
	eFragment = VK_SHADER_STAGE_FRAGMENT_BIT
};

enum class PrimitiveType
{
	ePoints = VK_PRIMITIVE_TOPOLOGY_POINT_LIST,
	eLines = VK_PRIMITIVE_TOPOLOGY_LINE_LIST,
	eTriangles = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
	eLineStrip = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP,
	eTriangleStrip = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST
};

class JDL_API VulkanPipeline : private NonCopyable<VulkanPipeline>
{
public:
	/**
	 * @brief Inits the pipeline, but does not create it.
	 */
	VulkanPipeline();

	/**
	 * @brief Destroys the pipeline.
	 */
	~VulkanPipeline();

	/**
	 * @brief Adds a shader to the pipeline.
	 * @param stage The pipeline stage for which the shader should be added.
	 * @param shader The shader resource.
	 */
	void addShader(ShaderStage stage, resource::Shader* shader);

	/**
	 * @brief Sets the pipeline primitive type.
	 * @param type The new primitive type.
	 */
	void setPrimitiveType(PrimitiveType type);

	/**
	 * @brief Creates the pipeline.
	 */
	void create();

	/**
	 * @brief Returns the Vulkan pipeline handle.
	 */
	VkPipeline getHandle() const {
		return m_pipeline;
	}

	/**
	 * @brief Returns the pipeline layout handle.
	 */
	VkPipelineLayout getPipelineLayout() const {
		return m_pipelineLayout;
	}

	/**
	 * @brief Returns the render pass handle.
	 */
	VkRenderPass getRenderPass() const {
		return m_renderPass;
	}

private:
	VkDevice m_device = VK_NULL_HANDLE;

	// Pipeline shaders
	std::unordered_map<ShaderStage, resource::Shader*> m_shaders;
	// Pipeline parameters.
	PrimitiveType m_primitiveType = PrimitiveType::eTriangles;

	// Pipeline Vulkan objects.
	VkPipeline m_pipeline = VK_NULL_HANDLE;
	VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;
	VkRenderPass m_renderPass = VK_NULL_HANDLE;

	void createPipelineLayout();
	void createRenderPass();
};

} // namespace vk
} // namespace jdl
