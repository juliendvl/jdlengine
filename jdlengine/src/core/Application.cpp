#include "core/Application.hpp"


namespace core
{

Application* Application::IApplication = nullptr;

Application::Application(const char* name, int width, int height)
    : m_name(name)
{
    if (IApplication != nullptr)
    {
        // TODO - Raise an error
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
    while (!m_window->isClosing())
    {
        m_window->pollEvents();
    }
}

} // namespace core
