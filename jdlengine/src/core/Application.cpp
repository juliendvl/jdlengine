#include "core/Application.hpp"
#include "core/VulkanContext.hpp"

#include "resource/ResourceManager.hpp"

#include "utils/Logger.hpp"


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
        JDL_FATAL("The application has already been created");
    }
    IApplication = this;

    // Create the window
    m_window = std::make_unique<Window>(name, width, height);
    // Init the Vulkan context
    VulkanContext::Init();
    // Create the renderer
    m_renderer = std::make_unique<Renderer>();
}

Application::~Application()
{
    // All the resources must be destroyed before destroying the VulkanContext!
    resource::ResourceManager::Clear();
    // Destroy the renderer
    m_renderer.reset();
    // Destroy the Vulkan context
    VulkanContext::Destroy();
    // Destroy the window
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

void Application::resizeEvent(const ResizeEvent& event)
{
    Size currentSize = event.getSize();
    while (currentSize.width == 0 || currentSize.height == 0)
    {
        m_window->waitEvents();
        currentSize = m_window->getFramebufferSize();
    }

    ResizeEvent newEvent(currentSize.width, currentSize.height);
    m_renderer->resizeEvent(newEvent);
}

} // namespace core
} // namespace jdl
