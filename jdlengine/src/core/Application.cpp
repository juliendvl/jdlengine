#include "core/Application.hpp"

#include "resource/ResourceManager.hpp"
#include "resource/Texture.hpp"

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

    createDefaultResources();
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

void Application::mousePressEvent(const MousePressEvent& event)
{
    m_renderer->mousePressEvent(event);
}

void Application::mouseReleaseEvent(const MouseReleaseEvent& event)
{
    m_renderer->mouseReleaseEvent(event);
}

void Application::mouseMoveEvent(const MouseMoveEvent& event)
{
    m_renderer->mouseMoveEvent(event);
}

void Application::wheelEvent(const WheelEvent& event)
{
    m_renderer->wheelEvent(event);
}

void Application::resizeEvent(const ResizeEvent& event)
{
    m_renderer->resizeEvent(event);
}

void Application::createDefaultResources()
{
    std::vector<unsigned char> texels {255, 255, 255, 255};
    auto texture = resource::ResourceManager::Create<resource::Texture>("__DEFAULT_TEXTURE__");
    texture->create(1, 1, texels.data());
}

} // namespace core
} // namespace jdl
