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

    m_window = std::make_unique<Window>(name, width, height);
}

Application::~Application()
{
    m_window.reset();
}

void Application::run()
{
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    while (m_window->isRunning())
    {
        glClear(GL_COLOR_BUFFER_BIT);
        m_window->pollEvents();
        m_window->swapBuffers();
    }
}

} // namespace core
} // namespace jdl
