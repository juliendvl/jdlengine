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
}

Application::~Application() {}

void Application::run() {}

} // namespace core
} // namespace jdl
