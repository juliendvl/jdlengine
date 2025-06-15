#include "core/Application.hpp"
#include "core/VulkanContext.hpp"

#include "utils/Logger.hpp"


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

    // Create the application window
    m_window = std::make_unique<Window>(name, width, height);
    // Initialize the Vulkan context
    VulkanContext::Init();
}

Application::~Application()
{
    // Destroy the Vulkan context
    VulkanContext::Destroy();
    // Destroy the application window
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
