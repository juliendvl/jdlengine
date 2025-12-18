#include "core/Window.hpp"

#include "utils/Logger.hpp"


namespace jdl
{
namespace core
{

Window* Window::IWindow = nullptr;

Window::Window(const std::string& title, size_t width, size_t height)
{
    if (IWindow != nullptr) {
        JDL_FATAL("The window has already been created");
    }
    IWindow = this;

    glfwSetErrorCallback(
        [](int error, const char* description) { JDL_ERROR("[GLFW {}] {}", error, description); }
    );

    int status = glfwInit();
    if (status == GLFW_FALSE) {
        JDL_FATAL("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    m_window = glfwCreateWindow((int)width, (int)height, title.c_str(), nullptr, nullptr);
    if (m_window == nullptr) {
        JDL_FATAL("Failed to create the GLFW window");
    }
}

Window::~Window()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

Size Window::getSize() const
{
    Size windowSize;
    glfwGetWindowSize(m_window, &windowSize.width, &windowSize.height);

    return windowSize;
}

Size Window::getFramebufferSize() const
{
    Size windowSize;
    glfwGetFramebufferSize(m_window, &windowSize.width, &windowSize.height);

    return windowSize;
}

std::vector<const char*> Window::GetInstanceExtensions()
{
    uint32_t nbExtensions = 0;
    const char** extensions = glfwGetRequiredInstanceExtensions(&nbExtensions);

    return std::vector<const char*>(extensions, extensions + nbExtensions);
}

} // namespace core
} // namespace jdl
