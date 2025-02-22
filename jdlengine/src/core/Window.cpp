#include "core/Window.hpp"
#include "core/Application.hpp"

#include "utils/Logger.hpp"


namespace jdl
{
namespace core
{

Window* Window::IWindow = nullptr;

Window::Window(const char* title, int width, int height)
    : m_window(nullptr)
{
    if (IWindow != nullptr)
    {
        JDL_FATAL("The window has already been created!");
    }
    IWindow = this;

    init(title, width, height);
}

Window::~Window()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

Size Window::getScreenSize() const
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

std::vector<const char*> Window::GetInstanceExtensions()
{
    uint32_t nbExtensions = 0;
    const char** extensions = glfwGetRequiredInstanceExtensions(&nbExtensions);

    return std::vector<const char*>(extensions, extensions + nbExtensions);
}

void Window::init(const char* title, int width, int height)
{
    // GLFW error callback
    glfwSetErrorCallback(
        [](int code, const char* description) { JDL_ERROR("GLFW {}: {}", code, description); }
    );

    int initStatus = glfwInit();
    if (initStatus == GLFW_FALSE)
    {
        JDL_FATAL("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (m_window == nullptr)
    {
        JDL_FATAL("Failed to create the GLFW window");
    }
    glfwSetWindowUserPointer(m_window, this);

    setupCallbacks();
}

void Window::setupCallbacks() {}

} // namespace core
} // namespace jdl
