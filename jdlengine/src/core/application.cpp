#include "core/application.hpp"


namespace jdl
{
namespace core
{

Application* Application::s_Application = nullptr;

Application::Application(const char* name, int width, int height)
    : m_name(name)
{
    if (s_Application != nullptr) {

    }
    s_Application = this;
}

Application::~Application() {}

void Application::run() {}

} // namespace core
} // namespace jdl
