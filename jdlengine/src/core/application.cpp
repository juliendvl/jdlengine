#include "core/application.hpp"

#include "utils/logger.hpp"


namespace jdl
{
namespace core
{

Application* Application::s_Application = nullptr;

Application::Application(const char* name, int width, int height)
    : m_name(name)
{
    if (s_Application != nullptr) {
        JDL_FATAL("The application has already been created");
    }
    s_Application = this;
}

Application::~Application() {}

void Application::run() {}

} // namespace core
} // namespace jdl
