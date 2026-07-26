#include "vk/vulkan_pipeline.hpp"

#include "resource/Shader.hpp"

#include "utils/logger.hpp"

#include "vk/vulkan_context.hpp"


namespace jdl
{
namespace vk
{

static const std::unordered_map<ShaderStage, const char*> s_ShaderEntryPoint {
	{ShaderStage::eVertex, "vert_main"},
	{ShaderStage::eFragment, "frag_main"}
};

static const std::vector<VkDynamicState> s_DynamicState = {
	VK_DYNAMIC_STATE_VIEWPORT,
	VK_DYNAMIC_STATE_SCISSOR
};

VulkanPipeline::VulkanPipeline()
{
	m_device = VulkanContext::GetDevice().get_device();
}

VulkanPipeline::~VulkanPipeline()
{
	if (m_pipelineLayout != VK_NULL_HANDLE) {
		vkDestroyPipelineLayout(m_device, m_pipelineLayout, nullptr);
	}
	if (m_pipeline != VK_NULL_HANDLE) {
		vkDestroyPipeline(m_device, m_pipeline, nullptr);
	}
}

void VulkanPipeline::add_shader(ShaderStage stage, resource::Shader* shader)
{
	if (m_pipeline != VK_NULL_HANDLE)
	{
		JDL_ERROR("Cannot add shader on a created pipeline");
		return;
	}
	m_shaders[stage] = shader;
}

void VulkanPipeline::create()
{
	if (m_pipeline != VK_NULL_HANDLE)
	{
		JDL_ERROR("Vulkan pipeline has already been created");
		return;
	}
	if (m_shaders.find(ShaderStage::eVertex) == m_shaders.end())
	{
		JDL_ERROR("Cannot create pipeline: missing vertex shader");
		return;
	}
	if (m_shaders.find(ShaderStage::eFragment) == m_shaders.end())
	{
		JDL_ERROR("Cannot create pipeline: missing fragment shader");
		return;
	}

	create_pipeline_layout();
	create_pipeline();
}

void VulkanPipeline::create_pipeline_layout()
{
	VkPipelineLayoutCreateInfo pipeline_layout_info {};
	pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

	VK_CALL(
		vkCreatePipelineLayout(
			m_device, &pipeline_layout_info, nullptr, &m_pipelineLayout
		)
	);
}

void VulkanPipeline::create_pipeline()
{
	VkSurfaceFormatKHR format = VulkanContext::GetSwapchain().get_surface_format();

	// Shaders
	std::vector<VkPipelineShaderStageCreateInfo> shader_infos;
	for (const auto& [stage, shader] : m_shaders)
	{
		VkPipelineShaderStageCreateInfo shader_info {};
		shader_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shader_info.stage = static_cast<VkShaderStageFlagBits>(stage);
		shader_info.module = shader->get_module();
		shader_info.pName = s_ShaderEntryPoint.at(stage);

		shader_infos.push_back(shader_info);
	}

	// Dynamic state
	VkPipelineDynamicStateCreateInfo dynamic_state {};
	dynamic_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamic_state.dynamicStateCount = VK_SIZE(s_DynamicState);
	dynamic_state.pDynamicStates = VK_DATA(s_DynamicState);

	// Vertex input
	VkPipelineVertexInputStateCreateInfo vertex_input {};
	vertex_input.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertex_input.vertexBindingDescriptionCount = 0;
	vertex_input.vertexAttributeDescriptionCount = 0;

	// Input assembly
	VkPipelineInputAssemblyStateCreateInfo input_assembly {};
	input_assembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	input_assembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	input_assembly.primitiveRestartEnable = VK_FALSE;

	// Viewport/Scissor
	VkPipelineViewportStateCreateInfo viewport_state {};
	viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewport_state.viewportCount = 1;
	viewport_state.scissorCount = 1;

	// Rasterizer
	VkPipelineRasterizationStateCreateInfo rasterizer {};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_NONE;
	rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;

	// Multisampling
	VkPipelineMultisampleStateCreateInfo multisampling {};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	// Color blending
	VkPipelineColorBlendAttachmentState color_blend_attachment {};
	color_blend_attachment.colorWriteMask = (
		VK_COLOR_COMPONENT_R_BIT |
		VK_COLOR_COMPONENT_G_BIT |
		VK_COLOR_COMPONENT_B_BIT |
		VK_COLOR_COMPONENT_A_BIT
	);
	color_blend_attachment.blendEnable = VK_FALSE;

	VkPipelineColorBlendStateCreateInfo color_blending {};
	color_blending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	color_blending.logicOpEnable = VK_FALSE;
	color_blending.attachmentCount = 1;
	color_blending.pAttachments = &color_blend_attachment;

	// Dynamic rendering
	VkPipelineRenderingCreateInfo rendering_info {};
	rendering_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
	rendering_info.colorAttachmentCount = 1;
	rendering_info.pColorAttachmentFormats = &format.format;

	VkGraphicsPipelineCreateInfo pipeline_info {};
	pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipeline_info.stageCount = VK_SIZE(shader_infos);
	pipeline_info.pStages = VK_DATA(shader_infos);
	pipeline_info.pVertexInputState = &vertex_input;
	pipeline_info.pInputAssemblyState = &input_assembly;
	pipeline_info.pViewportState = &viewport_state;
	pipeline_info.pRasterizationState = &rasterizer;
	pipeline_info.pMultisampleState = &multisampling;
	pipeline_info.pDepthStencilState = nullptr;
	pipeline_info.pColorBlendState = &color_blending;
	pipeline_info.pDynamicState = &dynamic_state;
	pipeline_info.layout = m_pipelineLayout;
	pipeline_info.pNext = &rendering_info;

	VK_CALL(
		vkCreateGraphicsPipelines(
			m_device, VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &m_pipeline
		)
	);
	
}

} // namespace vk
} // namespace jdl
