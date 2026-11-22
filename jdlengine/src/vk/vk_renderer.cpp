#include "vk/vk_renderer.hpp"


namespace jdl
{
namespace vk
{

Renderer::Renderer(const core::Window& window)
{
	m_context = std::make_unique<Context>(window);
}

Renderer::~Renderer()
{
	m_context.reset();
}

void Renderer::draw_frame() {}

} // namespace vk
} // namespace jdl
