#include "core/Application.hpp"

#include "resource/ResourceManager.hpp"

#include "utils/Logger.hpp"


namespace jdl
{
namespace core
{

Application* Application::IApplication = nullptr;

Application::Application(const std::string& name, size_t width, size_t height)
    : m_name(name)
{
    if (IApplication != nullptr) {
        JDL_FATAL("The application has already been created");
    }
    IApplication = this;

    // Create the application window
    m_window = std::make_unique<Window>(name, width, height);
    // Create the renderer
    m_renderer = std::make_unique<vk::VulkanRenderer>();
}

Application::~Application()
{
    // Wait for the renderer to finish all the work before deleting anything
    m_renderer->wait();
    // Clear all the resources
    resource::ResourceManager::Clear();
    // Destroy the renderer
    m_renderer.reset();
    // Destroy the application window
    m_window.reset();
}

void Application::run()
{
    while (m_window->isRunning())
    {
        m_window->pollEvents();
        m_renderer->renderFrame();
    }
}

} // namespace core
} // namespace jdl
