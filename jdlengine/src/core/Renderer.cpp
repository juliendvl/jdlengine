#include "core/Renderer.hpp"


namespace jdl
{
namespace core
{

Renderer::Renderer() {}

Renderer::~Renderer() {}

void Renderer::renderFrame()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Renderer::resizeEvent(const ResizeEvent& event)
{
    glViewport(0, 0, event.getWidth(), event.getHeight());
}

} // namespace core
} // namespace jdl
