#include "core/Renderer.hpp"

#include "utils/Logger.hpp"


namespace jdl
{
namespace core
{

Renderer::Renderer() {}

Renderer::~Renderer() {}

void Renderer::setBackgroundColor(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
}

void Renderer::renderFrame()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

} // namespace core
} // namespace jdl
