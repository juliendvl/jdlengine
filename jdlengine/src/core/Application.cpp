#include "core/Application.hpp"

#include "resource/ShaderProgram.hpp"

#include "utils/Logger.hpp"


namespace jdl
{
namespace core
{

Application* Application::IApplication = nullptr;

Application::Application(const char* name)
    : m_name(name)
{
    if (IApplication != nullptr)
    {
        JDL_FATAL("An application has already been created!");
    }
    IApplication = this;

    m_window = std::make_unique<Window>(name, 800, 600);

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
    while (!m_window->shouldClose())
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
        "default_shader",
        "resources/shaders/default.vert",
        "resources/shaders/default.frag"
    );
    shader->use();
}

} // namespace core
} // namespace jdl
