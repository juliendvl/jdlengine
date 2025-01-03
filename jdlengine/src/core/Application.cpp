#include "core/Application.hpp"

#include "resource/ShaderProgram.hpp"
#include "resource/Texture.hpp"

#include "utils/Logger.hpp"


namespace jdl
{
namespace core
{

Application* Application::IApplication = nullptr;


Application::Application(const char* name, int width, int height)
    : m_name(name)
{
    if (IApplication != nullptr)
    {
        JDL_FATAL("An Application already exists");
    }
    IApplication = this;

    m_window = std::make_unique<Window>(name, width, height);

    createDefaultResources();

    m_renderer = std::make_unique<Renderer>();
    m_renderer->setBackgroundColor(0.2f, 0.2f, 0.2f);
}

Application::~Application()
{
    m_renderer.reset();
    m_window.reset();
}

void Application::run()
{
    while (m_window->isRunning())
    {
        m_window->pollEvents();
        m_renderer->renderFrame();
        m_window->swapBuffers();
    }
}

void Application::mousePressEvent(const MousePressEvent& event)
{
    m_renderer->mousePressEvent(event);
}

void Application::mouseMoveEvent(const MouseMoveEvent& event)
{
    m_renderer->mouseMoveEvent(event);
}

void Application::mouseReleaseEvent(const MouseReleaseEvent& event)
{
    m_renderer->mouseReleaseEvent(event);
}

void Application::wheelEvent(const WheelEvent& event)
{
    m_renderer->wheelEvent(event);
}

void Application::resizeEvent(const ResizeEvent& event)
{
    m_renderer->resizeEvent(event);
}

void Application::createDefaultResources()
{
    auto shader = resource::ResourceManager::Create<resource::ShaderProgram>(
        "__DEFAULT_SHADER__",
        "shaders/pbr.vert",
        "shaders/pbr.frag"
    );
    shader->use();

    unsigned char texels[] = {255, 255, 255};
    auto texture = resource::ResourceManager::Create<resource::Texture>("__WHITE_TEXTURE__", 1, 1, 3, texels);
}

} // namespace core
} // namespace jdl
