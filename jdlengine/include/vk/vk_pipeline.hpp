#pragma once

#include "utils/non_copyable.hpp"

#include "vk_common.hpp"


namespace jdl
{
namespace vk
{

class Device;
class Swapchain;

class Pipeline : private utils::NonCopyable<Pipeline>
{
public:
	/**
	 * @brief Creates the graphics pipeline and its layout, for dynamic rendering.
	 * Viewport and scissor are dynamic states and must be set when recording.
	 * @param device The device the pipeline is created for.
	 * @param swapchain The swapchain whose image format is used as color attachment format.
	 */
	Pipeline(const Device& device, const Swapchain& swapchain);

	/**
	 * @brief Destroys the pipeline and its layout.
	 */
	~Pipeline();

	/**
	 * @brief Returns the pipeline layout handle.
	 */
	VkPipelineLayout pipeline_layout() const { return m_pipelineLayout; }

	/**
	 * @brief Returns the pipeline handle.
	 */
	VkPipeline handle() const { return m_pipeline; }

private:
	VK_ATTR(VkDevice, m_device);
	VK_ATTR(VkPipelineLayout, m_pipelineLayout);
	VK_ATTR(VkPipeline, m_pipeline);

	void create_pipeline_layout();
	void create_pipeline(const Device& device, const Swapchain& swapchain);
};

} // namespace vk
} // namespace jdl
