#include "vulkan/VulkanPipeline.hpp"
#include "vulkan/VulkanContext.hpp"

#include "core/Vertex.hpp"

#include "utils/Logger.hpp"


namespace jdl
{
namespace vk
{

static const uint32_t MAX_SETS = 50;

static std::vector<VkVertexInputBindingDescription> s_GetVertexBindings()
{
	VkVertexInputBindingDescription bindingDescription {};
	bindingDescription.binding = 0;
	bindingDescription.stride = sizeof(core::Vertex);
	bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	return { bindingDescription };
}

static std::vector<VkVertexInputAttributeDescription> s_GetVertexAttributes()
{
	// Position
	VkVertexInputAttributeDescription positionDescription {};
	positionDescription.binding = 0;
	positionDescription.location = 0;
	positionDescription.format = VK_FORMAT_R32G32B32_SFLOAT;
	positionDescription.offset = offsetof(core::Vertex, position);

	// Position
	VkVertexInputAttributeDescription normalDescription {};
	normalDescription.binding = 0;
	normalDescription.location = 1;
	normalDescription.format = VK_FORMAT_R32G32B32_SFLOAT;
	normalDescription.offset = offsetof(core::Vertex, normal);

	// Position
	VkVertexInputAttributeDescription uvDescription {};
	uvDescription.binding = 0;
	uvDescription.location = 2;
	uvDescription.format = VK_FORMAT_R32G32_SFLOAT;
	uvDescription.offset = offsetof(core::Vertex, uv);

	return { positionDescription, normalDescription, uvDescription };
}


VulkanPipeline::VulkanPipeline()
{
	m_device = VulkanContext::GetDevice().get();
}

VulkanPipeline::~VulkanPipeline()
{
	if (m_pipeline)
	{
		for (const auto& pool : m_descriptorPools) {
			vkDestroyDescriptorPool(m_device, pool, nullptr);
		}
		m_descriptorPools.clear();

		for (const auto& [layout, handle] : m_descriptorSetLayouts) {
			vkDestroyDescriptorSetLayout(m_device, handle, nullptr);
		}
		m_descriptorSetLayouts.clear();

		vkDestroyRenderPass(m_device, m_renderPass, nullptr);
		vkDestroyPipelineLayout(m_device, m_pipelineLayout, nullptr);
		vkDestroyPipeline(m_device, m_pipeline, nullptr);
	}
}

void VulkanPipeline::addShader(ShaderStage stage, resource::Shader* shader)
{
	if (m_pipeline)
	{
		JDL_ERROR("Cannot add shader on built pipeline.");
		return;
	}
	m_shaders[stage] = shader;
}

void VulkanPipeline::setPrimitiveType(PrimitiveType type)
{
	if (m_pipeline)
	{
		JDL_ERROR("Cannot set primitive type on built pipeline.");
		return;
	}
	m_primitiveType = type;
}

void VulkanPipeline::create()
{
	// Dynamic states
	std::vector<VkDynamicState> dynamicStates = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

	VkPipelineDynamicStateCreateInfo dynamicState {};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
	dynamicState.pDynamicStates = dynamicStates.data();

	// Shaders
	if (m_shaders.find(ShaderStage::eVertex) == m_shaders.end())
	{
		JDL_ERROR("Cannot create pipeline: Missing Vertex shader");
		return;
	}
	if (m_shaders.find(ShaderStage::eFragment) == m_shaders.end())
	{
		JDL_ERROR("Cannot create pipeline: Missing Fragment shader");
		return;
	}

	std::vector<VkPipelineShaderStageCreateInfo> shaderInfos;
	for (const auto& [stage, shader] : m_shaders)
	{
		VkPipelineShaderStageCreateInfo shaderInfo {};
		shaderInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderInfo.stage = static_cast<VkShaderStageFlagBits>(stage);
		shaderInfo.module = shader->getModule();
		shaderInfo.pName = "main";

		shaderInfos.push_back(shaderInfo);
	}

	// Vertex input
	auto bindings = s_GetVertexBindings();
	auto attributes = s_GetVertexAttributes();

	VkPipelineVertexInputStateCreateInfo vertexInputInfo {};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindings.size());
	vertexInputInfo.pVertexBindingDescriptions = bindings.data();
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributes.size());
	vertexInputInfo.pVertexAttributeDescriptions = attributes.data();

	// Input assembly
	VkPipelineInputAssemblyStateCreateInfo inputAssembly {};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = static_cast<VkPrimitiveTopology>(m_primitiveType);

	// Viewport and Scissor
	VkPipelineViewportStateCreateInfo viewportState {};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.scissorCount = 1;

	// Rasterizer
	VkPipelineRasterizationStateCreateInfo rasterizer {};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_NONE;
	rasterizer.depthBiasEnable = VK_FALSE;

	// Multisampling
	VkPipelineMultisampleStateCreateInfo multisampling {};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	// Color blending
	VkPipelineColorBlendAttachmentState colorBlendAttachment {};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT
		| VK_COLOR_COMPONENT_G_BIT
		| VK_COLOR_COMPONENT_B_BIT
		| VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;

	VkPipelineColorBlendStateCreateInfo colorBlending {};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;

	// Pipeline layout
	createPipelineLayout();
	// Create a first descriptor pool
	createDescriptorPool();
	// Render pass
	createRenderPass();

	VkGraphicsPipelineCreateInfo pipelineInfo {};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = static_cast<uint32_t>(shaderInfos.size());
	pipelineInfo.pStages = shaderInfos.data();
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = nullptr;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = &dynamicState;
	pipelineInfo.layout = m_pipelineLayout;
	pipelineInfo.renderPass = m_renderPass;
	pipelineInfo.subpass = 0;

	VK_CALL(vkCreateGraphicsPipelines(m_device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_pipeline));
}

void VulkanPipeline::bind(VkCommandBuffer commandBuffer) const
{
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);
}

VkDescriptorSet VulkanPipeline::allocateDescriptorSet(DescriptorLayoutPreset layoutPreset)
{
	VkDescriptorSet set = VK_NULL_HANDLE;

	VkDescriptorSetAllocateInfo allocateInfo {};
	allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocateInfo.descriptorPool = m_descriptorPools.back();
	allocateInfo.descriptorSetCount = 1;
	allocateInfo.pSetLayouts = &m_descriptorSetLayouts[layoutPreset];

	VkResult result = vkAllocateDescriptorSets(m_device, &allocateInfo, &set);
	if (result == VK_ERROR_OUT_OF_POOL_MEMORY || result == VK_ERROR_FRAGMENTED_POOL)
	{
		// The descriptor pool is full; create a new one and retry
		createDescriptorPool();
		return allocateDescriptorSet(layoutPreset);
	}
	else if (result != VK_SUCCESS) {
		JDL_FATAL("Failed to allocate the descriptor set");
	}

	return set;
}

void VulkanPipeline::createPipelineLayout()
{
	// Descriptor set layout (global)
	VkDescriptorSetLayoutBinding uboLayoutBinding{};
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.descriptorCount = 1;
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

	VkDescriptorSetLayoutCreateInfo globalLayoutInfo{};
	globalLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	globalLayoutInfo.bindingCount = 1;
	globalLayoutInfo.pBindings = &uboLayoutBinding;

	VkDescriptorSetLayout globalLayout;
	VK_CALL(vkCreateDescriptorSetLayout(m_device, &globalLayoutInfo, nullptr, &globalLayout));
	m_descriptorSetLayouts[DescriptorLayoutPreset::eGlobal] = globalLayout;

	// Push constants (model matrix)
	VkPushConstantRange pushConstantRange{};
	pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	pushConstantRange.offset = 0;
	pushConstantRange.size = sizeof(glm::mat4);

	VkPipelineLayoutCreateInfo pipelineLayoutInfo {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 1;
	pipelineLayoutInfo.pSetLayouts = &globalLayout;
	pipelineLayoutInfo.pushConstantRangeCount = 1;
	pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

	VK_CALL(vkCreatePipelineLayout(m_device, &pipelineLayoutInfo, nullptr, &m_pipelineLayout));
}

void VulkanPipeline::createRenderPass()
{
	auto& swapChain = VulkanContext::GetSwapChain();

	// Attachments
	VkAttachmentDescription colorAttachment {};
	colorAttachment.format = swapChain.getSurfaceFormat().format;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	std::vector<VkAttachmentDescription> attachments { colorAttachment };

	// Subpasses
	VkAttachmentReference colorAttachmentRef {};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;

	VkRenderPassCreateInfo renderPassInfo {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	renderPassInfo.pAttachments = attachments.data();
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;

	VK_CALL(vkCreateRenderPass(m_device, &renderPassInfo, nullptr, &m_renderPass));
}

void VulkanPipeline::createDescriptorPool()
{
	VkDescriptorPoolSize poolSize {};
	poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSize.descriptorCount = MAX_SETS;

	VkDescriptorPoolCreateInfo poolInfo {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = 1;
	poolInfo.pPoolSizes = &poolSize;
	poolInfo.maxSets = MAX_SETS;

	VkDescriptorPool pool;
	VK_CALL(vkCreateDescriptorPool(m_device, &poolInfo, nullptr, &pool));
	m_descriptorPools.push_back(pool);
}

} // namespace vk
} // namespace jdl
