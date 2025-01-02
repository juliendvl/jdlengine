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

void Renderer::mousePressEvent(const MousePressEvent& event)
{
    if (m_scene)
    {
        m_scene->mousePressEvent(event);
    }
}

void Renderer::mouseMoveEvent(const MouseMoveEvent& event)
{
    if (m_scene)
    {
        m_scene->mouseMoveEvent(event);
    }
}

void Renderer::mouseReleaseEvent(const MouseReleaseEvent& event)
{
    if (m_scene)
    {
        m_scene->mouseReleaseEvent(event);
    }
}

void Renderer::wheelEvent(const WheelEvent& event)
{
    if (m_scene)
    {
        m_scene->wheelEvent(event);
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
