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
}

Application::~Application() {}

void Application::run() {}

} // namespace core
} // namespace jdl
