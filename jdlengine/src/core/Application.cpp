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

    JDL_INFO("Create the application window");
    m_window = std::make_unique<Window>(name, 800, 600);
}

Application::~Application()
{
    JDL_INFO("Destroy the application window");
    m_window.reset();
}

void Application::run()
{
    while (!m_window->shouldClose())
    {
        m_window->pollEvents();
        glClear(GL_COLOR_BUFFER_BIT);
        m_window->swapBuffers();
    }
}

} // namespace core
} // namespace jdl
