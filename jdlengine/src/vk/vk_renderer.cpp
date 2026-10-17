#include "vk/vk_renderer.hpp"


namespace jdl
{
namespace vk
{

Renderer::Renderer()
{
	m_context = std::make_unique<Context>();
}

Renderer::~Renderer()
{
	m_context.reset();
}

void Renderer::draw_frame() {}

} // namespace vk
} // namespace jdl
