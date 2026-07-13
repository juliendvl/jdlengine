#include "core/application.hpp"

#include "utils/logger.hpp"


namespace jdl
{
namespace core
{

Application* Application::s_Application = nullptr;
const char* Application::s_Name = nullptr;

Application::Application(const char* name, int width, int height)
{
    if (s_Application != nullptr) {
        JDL_FATAL("The application has already been created");
    }
    s_Application = this;
    s_Name = name;

    m_window = std::make_unique<Window>(name, width, height);
}

Application::~Application()
{
    m_window.reset();
}

void Application::run() {}

} // namespace core
} // namespace jdl
