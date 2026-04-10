#include "vk/VulkanRenderer.hpp"
#include "vk/VulkanContext.hpp"


namespace jdl
{
namespace vk
{

VulkanRenderer::VulkanRenderer()
{
	VulkanContext::Init();
}

VulkanRenderer::~VulkanRenderer()
{
	VulkanContext::Destroy();
}

void VulkanRenderer::renderFrame() {}

} // namespace vk
} // namespace jdl
