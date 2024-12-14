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
        JDL_FATAL("An Application already exists");
    }
    IApplication = this;

    m_window = std::make_unique<Window>(name, width, height);
}

Application::~Application()
{
    m_window.reset();
}

void Application::run()
{
    while (m_window->isRunning())
    {
        m_window->pollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_window->swapBuffers();
    }
}

} // namespace core
} // namespace jdl
