#include "core/Application.hpp"


namespace jdl
{
namespace core
{

Application* Application::IApplication = nullptr;
const char* Application::IName = nullptr;


Application::Application(const char* name, int width, int height)
{
	if (IApplication != nullptr) {
		// TODO - Raise a fatal error
	}
	IApplication = this;
	IName = name;

	m_window = std::make_unique<Window>(name, width, height);
}

Application::~Application()
{
	m_window.reset();
}

void Application::run()
{
	while (m_window->isRunning())
	{
		m_window->pollEvents();
	}
}

} // namespace core
} // namespace jdl
