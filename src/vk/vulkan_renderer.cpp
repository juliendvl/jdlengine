#include "vk/vulkan_renderer.hpp"

#include "vk/vulkan_context.hpp"


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

void VulkanRenderer::render_frame() {}

} // namespace vk
} // namespace jdl
