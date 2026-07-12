#include "core/application.hpp"


namespace jdl
{
namespace core
{

Application* Application::s_Application = nullptr;
const char* Application::s_Name = nullptr;

Application::Application(const char* name, int width, int height)
{
    if (s_Application != nullptr) {
        // TODO Raise a fatal error
    }
    s_Application = this;
    s_Name = name;
}

Application::~Application() {}

void Application::run() {}

} // namespace core
} // namespace jdl
