#include "core/Renderer.hpp"


namespace jdl
{
namespace core
{

Renderer::Renderer()
{
    m_context.shaderProgram = resource::ResourceManager::As<resource::ShaderProgram>("DefaultShader");
}

Renderer::~Renderer() {}

void Renderer::renderFrame()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    if (m_scene != nullptr)
    {
        m_scene->render(m_context);
    }
}

void Renderer::resizeEvent(const ResizeEvent& event)
{
    glViewport(0, 0, event.getWidth(), event.getHeight());
    if (m_scene != nullptr)
    {
        m_scene->resizeEvent(event);
    }
}

} // namespace core
} // namespace jdl
