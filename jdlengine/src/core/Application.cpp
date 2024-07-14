#include "core/Application.hpp"

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

void Application::resizeEvent(const ResizeEvent& event)
{
    m_renderer->resizeEvent(event);
}

} // namespace core
} // namespace jdl
