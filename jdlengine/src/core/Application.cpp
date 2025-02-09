#include "core/Application.hpp"


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
        // TODO - Raise a Fatal Error
    }
    IApplication = this;

    // Create the window
    m_window = std::make_unique<Window>(name, width, height);
    // Create the renderer
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

void Application::resizeEvent(const ResizeEvent& event)
{
    m_renderer->resizeEvent(event);
}

} // namespace core
} // namespace jdl
