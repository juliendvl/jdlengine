#include "core/Window.hpp"

#include "utils/Logger.hpp"


namespace jdl
{
namespace core
{

Window* Window::IWindow = nullptr;

Window::Window(const char* name, int width, int height)
    : m_window(nullptr)
{
    if (IWindow != nullptr)
    {
        JDL_FATAL("Only one window is supported!");
    }
    IWindow = this;

    glfwSetErrorCallback(
        [](int error, const char* description) { JDL_ERROR("GLFW {}: {}", error, description); }
    );

    int initStatus = glfwInit();
    if (initStatus == GLFW_FALSE)
    {
        JDL_FATAL("Failed to init GLFW!");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifndef NDEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

    m_window = glfwCreateWindow(width, height, name, nullptr, nullptr);
    if (m_window == nullptr)
    {
        JDL_FATAL("Failed to create the GLFW window!");
    }
    glfwMakeContextCurrent(m_window);

    initStatus = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    if (initStatus == 0)
    {
        JDL_FATAL("Failed to init GLAD!");
    }

    glViewport(0, 0, width, height);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    setupCallbacks();
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

bool Window::isRunning() const
{
    int shouldClose = glfwWindowShouldClose(m_window);
    return shouldClose == GLFW_FALSE;
}

void Window::setupCallbacks()
{
    glfwSetWindowSizeCallback(
        m_window,
        [](GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); }
    );
}

} // namespace core
} // namespace jdl
