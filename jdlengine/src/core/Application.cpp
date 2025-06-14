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
}

Application::~Application() {}

void Application::run() {}

} // namespace core
