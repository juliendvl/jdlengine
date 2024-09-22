#include "core/Renderer.hpp"

#include "resource/ShaderProgram.hpp"


namespace jdl
{
namespace core
{

Renderer::Renderer()
    : m_context()
{
    // Init the context
    m_context.shaderProgram = resource::ResourceManager::As<resource::ShaderProgram>("default_shader");
}

Renderer::~Renderer() {}

void Renderer::renderFrame()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    if (m_scene)
    {
        m_scene->render(m_context);
    }
}

void Renderer::resizeEvent(const ResizeEvent& event)
{
    glViewport(0, 0, event.getWidth(), event.getHeight());
}

} // namespace core
} // namespace jdl
