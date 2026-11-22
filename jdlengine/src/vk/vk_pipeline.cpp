#include "vk/vk_pipeline.hpp"

#include <array>
#include <fstream>
#include <vector>

#include "vk/vk_device.hpp"
#include "vk/vk_shader_module.hpp"
#include "vk/vk_swapchain.hpp"


namespace jdl
{
namespace vk
{

namespace
{

// Reads a SPIR-V binary file into 32-bit words, ready to be passed to ShaderModule.
std::vector<uint32_t> ReadShader(const char* path)
{
	// Open at the end of the file so that tellg() gives the file size.
	std::ifstream stream(path, std::ios::binary | std::ios::ate);
	if (!stream) {
		JDL_FATAL("Failed to open shader {}", path);
	}

	// SPIR-V is a stream of 32-bit words: any other size means a truncated or invalid file.
	const std::streamsize size = stream.tellg();
	if (size <= 0 || static_cast<size_t>(size) % sizeof(uint32_t) != 0) {
		JDL_FATAL("Invalid shader size for {}: {} bytes", path, static_cast<long long>(size));
	}
	stream.seekg(0);

	// Reading into uint32_t storage guarantees the 4-byte alignment required by pCode.
	std::vector<uint32_t> code(static_cast<size_t>(size) / sizeof(uint32_t));
	if (!stream.read(reinterpret_cast<char*>(code.data()), size)) {
		JDL_FATAL("Failed to read shader {}", path);
	}
	return code;
}

} // namespace

Pipeline::Pipeline(const Device& device, const Swapchain& swapchain)
	: m_device(device.device())
{
	create_pipeline_layout();
	create_pipeline(device, swapchain);

	JDL_INFO("Created graphics pipeline");
}

Pipeline::~Pipeline()
{
	vkDestroyPipeline(m_device, m_pipeline, nullptr);
	vkDestroyPipelineLayout(m_device, m_pipelineLayout, nullptr);
}

void Pipeline::create_pipeline_layout()
{
	VkPipelineLayoutCreateInfo create_info {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO
	};
	VK_CHECK(vkCreatePipelineLayout(m_device, &create_info, nullptr, &m_pipelineLayout));
}

void Pipeline::create_pipeline(const Device& device, const Swapchain& swapchain)
{
	// Shader Modules
	// TODO - Temporary: shaders are hardcoded for now.
	// TODO - Use a more generic method in the future.
	auto shader_code = ReadShader("shaders/default.spv");
	ShaderModule shader(device, shader_code);

	VkPipelineShaderStageCreateInfo vertex_shader_info {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
		.stage = VK_SHADER_STAGE_VERTEX_BIT,
		.module = shader.handle(),
		.pName = "vert_main"
	};
	VkPipelineShaderStageCreateInfo fragment_shader_info {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
		.stage = VK_SHADER_STAGE_FRAGMENT_BIT,
		.module = shader.handle(),
		.pName = "frag_main"
	};

	std::array<VkPipelineShaderStageCreateInfo, 2> shader_stages {
		vertex_shader_info, fragment_shader_info
	};

	// Dynamic state
	std::array<VkDynamicState, 2> dynamic_states {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR,
	};

	VkPipelineDynamicStateCreateInfo dynamic_state {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
		.dynamicStateCount = VK_SIZE(dynamic_states),
		.pDynamicStates = VK_DATA(dynamic_states)
	};

	// Vertex input
	VkPipelineVertexInputStateCreateInfo vertex_input {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO
	};

	// Input assembly
	VkPipelineInputAssemblyStateCreateInfo input_assembly {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
		.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
		.primitiveRestartEnable = VK_FALSE
	};

	// Viewport/Scissor
	VkPipelineViewportStateCreateInfo viewport_state {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
		.viewportCount = 1,
		.scissorCount = 1
	};

	// Rasterizer
	VkPipelineRasterizationStateCreateInfo rasterizer {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
		.depthClampEnable = VK_FALSE,
		.rasterizerDiscardEnable = VK_FALSE,
		.polygonMode = VK_POLYGON_MODE_FILL,
		.cullMode = VK_CULL_MODE_NONE,
		.frontFace = VK_FRONT_FACE_CLOCKWISE,
		.depthBiasEnable = VK_FALSE,
		.lineWidth = 1.0f
	};

	// Multisampling
	VkPipelineMultisampleStateCreateInfo multisampling {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
		.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
		.sampleShadingEnable = VK_FALSE
	};

	// Color blending
	VkPipelineColorBlendAttachmentState color_blend_attachment {
		.blendEnable = VK_FALSE,
		.colorWriteMask = (
			VK_COLOR_COMPONENT_R_BIT
			| VK_COLOR_COMPONENT_G_BIT
			| VK_COLOR_COMPONENT_B_BIT
			| VK_COLOR_COMPONENT_A_BIT
		)
	};

	VkPipelineColorBlendStateCreateInfo color_blending {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
		.logicOpEnable = VK_FALSE,
		.attachmentCount = 1,
		.pAttachments = &color_blend_attachment
	};

	// Dynamic rendering
	VkFormat format = swapchain.image_format();
	VkPipelineRenderingCreateInfo rendering_info {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO,
		.colorAttachmentCount = 1,
		.pColorAttachmentFormats = &format
	};

	VkGraphicsPipelineCreateInfo pipeline_info {
		.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
		.pNext = &rendering_info,
		.stageCount = VK_SIZE(shader_stages),
		.pStages = VK_DATA(shader_stages),
		.pVertexInputState = &vertex_input,
		.pInputAssemblyState = &input_assembly,
		.pViewportState = &viewport_state,
		.pRasterizationState = &rasterizer,
		.pMultisampleState = &multisampling,
		.pDepthStencilState = nullptr,
		.pColorBlendState = &color_blending,
		.pDynamicState = &dynamic_state,
		.layout = m_pipelineLayout,
		.renderPass = VK_NULL_HANDLE
	};
	VK_CHECK(
		vkCreateGraphicsPipelines(
			m_device, VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &m_pipeline
		)
	);
}

} // namespace vk
} // namespace jdl
