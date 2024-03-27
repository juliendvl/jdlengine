#include "core/Application.hpp"

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
        JDL_FATAL("An application already exists!");
    }
    IApplication = this;

    JDL_INFO("Application - Create window");
    m_window = std::make_unique<Window>(name, width, height);

    JDL_INFO("Application - Create renderer");
    m_renderer = std::make_unique<Renderer>();
}

Application::~Application()
{
    JDL_INFO("Application - Destroy renderer");
    m_renderer.reset();

    JDL_INFO("Application - Destroy window");
    m_window.reset();
}

void Application::run()
{
    while (m_window->isRunning())
    {
        m_window->swapBuffers();
        m_window->pollEvents();
        m_renderer->renderFrame();
    }
}

} // namespace core
} // namespace jdl
