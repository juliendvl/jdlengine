#include "core/Application.hpp"


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
        // TODO - Log a Fatal error
    }
    IApplication = this;
}

void Application::run() {}

} // namespace core
} // namespace jdl
