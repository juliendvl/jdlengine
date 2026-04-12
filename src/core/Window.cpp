#include "core/Window.hpp"

#include "utils/Logger.hpp"

#include "vk/VulkanContext.hpp"


namespace jdl
{
namespace core
{

Window* Window::IWindow = nullptr;

Window::Window(const char* title, int width, int height)
{
	if (IWindow != nullptr) {
		JDL_FATAL("Window has already been created!");
	}
	IWindow = this;

	glfwSetErrorCallback(
		[](int error, const char* description) {
			JDL_ERROR("[GLFW {}] {}", error, description);
		}
	);

	auto status = glfwInit();
	if (status == GLFW_FALSE) {
		JDL_FATAL("Failed to initialize GLFW!");
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (!m_window) {
		JDL_FATAL("Failed to create the GLFW window!");
	}
	glfwSetWindowUserPointer(m_window, this);
}

Window::~Window()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

Size Window::getSize() const
{
	Size size;
	glfwGetWindowSize(m_window, &size.width, &size.height);

	return size;
}

Size Window::getFramebufferSize() const
{
	Size size;
	glfwGetFramebufferSize(m_window, &size.width, &size.height);

	return size;
}

bool Window::isRunning() const
{
	auto status = glfwWindowShouldClose(m_window);
	return status == GLFW_FALSE;
}

std::vector<const char*> Window::GetRequiredInstanceExtensions()
{
	uint32_t nbExtensions = 0;
	const char** extensions = glfwGetRequiredInstanceExtensions(&nbExtensions);

	return std::vector<const char*>{extensions, extensions + nbExtensions};
}

VkSurfaceKHR Window::createWindowSurface() const
{
	VkSurfaceKHR surface = VK_NULL_HANDLE;
	VK_CALL(glfwCreateWindowSurface(
		vk::VulkanContext::GetInstance().get(),
		m_window,
		nullptr,
		&surface
	));

	return surface;
}


} // namespace core
} // namespace jdl
