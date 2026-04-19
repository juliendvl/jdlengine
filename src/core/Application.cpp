#include "core/Application.hpp"

#include "resource/ResourceManager.hpp"

#include "utils/Logger.hpp"


namespace jdl
{
namespace core
{

Application* Application::IApplication = nullptr;
const char* Application::IName = nullptr;


Application::Application(const char* name, int width, int height)
{
	if (IApplication != nullptr) {
		JDL_FATAL("The application has already been created");
	}
	IApplication = this;
	IName = name;

	m_window = std::make_unique<Window>(name, width, height);
	m_renderer = std::make_unique<vk::VulkanRenderer>();
}

Application::~Application()
{
	resource::ResourceManager::Clear();

	m_renderer.reset();
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
