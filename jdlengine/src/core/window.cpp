#include "core/window.hpp"

#include "utils/logger.hpp"


namespace jdl
{
namespace core
{

Window* Window::s_Window = nullptr;

Window::Window(const char* title, int width, int height)
{
	if (s_Window != nullptr) {
		JDL_FATAL("The window has already been created");
	}
	s_Window = this;

	glfwSetErrorCallback(
		[](int error, const char* description) {
			JDL_ERROR("GLFW {}: {}", error, description);
		}
	);

	int status = glfwInit();
	if (status == GLFW_FALSE) {
		JDL_FATAL("Failed to initialize GLFW");
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (m_window == nullptr) {
		JDL_FATAL("Failed to create the GLFW window");
	}
}

Window::~Window()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

Size Window::screen_size() const
{
	Size size;
	glfwGetWindowSize(m_window, &size.width, &size.height);
	return size;
}

Size Window::framebuffer_size() const
{
	Size size;
	glfwGetFramebufferSize(m_window, &size.width, &size.height);
	return size;
}

std::vector<const char*> Window::GetInstanceExtensions()
{
	uint32_t nb_extensions;

	const char** extensions = glfwGetRequiredInstanceExtensions(&nb_extensions);
	return std::vector<const char*>(extensions, extensions + nb_extensions);
}

} // namespace core
} // namespace jdl
