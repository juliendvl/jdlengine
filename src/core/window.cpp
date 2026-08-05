#include "core/window.hpp"

#include "core/application.hpp"
#include "core/events.hpp"

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

    if (glfwInit() == GLFW_FALSE) {
        JDL_FATAL("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (m_window == nullptr) {
        JDL_FATAL("Failed to create the GLFW window");
    }
    glfwSetWindowUserPointer(m_window, s_Window);

    setup_callbacks();
}

Window::~Window()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

Size Window::get_size() const
{
    Size size;
    glfwGetWindowSize(m_window, &size.width, &size.height);

    return size;
}

Size Window::get_framebuffer_size() const
{
    Size size;
    glfwGetFramebufferSize(m_window, &size.width, &size.height);

    return size;
}

std::vector<const char*> Window::GetRequiredInstanceExtensions()
{
    uint32_t nb_extensions;
    const char** extensions = glfwGetRequiredInstanceExtensions(&nb_extensions);

    return std::vector<const char*>(extensions, extensions + nb_extensions);
}

VkSurfaceKHR Window::create_window_surface(VkInstance instance) const
{
    VkSurfaceKHR surface = VK_NULL_HANDLE;
    VK_CALL(glfwCreateWindowSurface(instance, m_window, nullptr, &surface));

    return surface;
}

void Window::setup_callbacks()
{
    // Resize Callback
    glfwSetWindowSizeCallback(
        m_window,
        [](GLFWwindow* window, int width, int height)
        {
            // Handle window minimization
            while (width == 0 || height == 0)
            {
                glfwWaitEvents();
                glfwGetWindowSize(window, &width, &height);
            }

            ResizeEvent event(width, height);
            Application::Get().resize_event(event);
        }
    );
}

} // namespace core
} // namespace jdl
