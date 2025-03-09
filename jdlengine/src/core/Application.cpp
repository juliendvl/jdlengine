#include "core/Application.hpp"
#include "core/VulkanContext.hpp"

#include "resource/ResourceManager.hpp"
#include "resource/Shader.hpp"

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

    createDefaultResources();
}

Application::~Application()
{
    // All the resources must be destroyed before destroying the VulkanContext!
    resource::ResourceManager::Clear();
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
    }
}

void Application::createDefaultResources()
{
    // Default PBR shaders
    resource::ResourceManager::Create<resource::Shader>("PBR_VERT", "shaders/pbr_vert.spv");
    resource::ResourceManager::Create<resource::Shader>("PBR_FRAG", "shaders/pbr_frag.spv");
}

} // namespace core
} // namespace jdl
