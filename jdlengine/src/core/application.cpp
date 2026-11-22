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

    m_window = std::make_unique<Window>(name, width, height);
    JDL_INFO("Created application window");

    m_renderer = std::make_unique<vk::Renderer>(*m_window);
    JDL_INFO("Created Vulkan renderer");
}

Application::~Application()
{
    // The renderer uses the window surface: destroy it first.
    m_renderer.reset();
    m_window.reset();
    s_Application = nullptr;
}

void Application::run()
{
    while (m_window->is_running())
    {
        m_window->poll_events();
        m_renderer->draw_frame();
    }
}

} // namespace core
} // namespace jdl
