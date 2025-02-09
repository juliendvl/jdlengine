#include "core/Window.hpp"
#include "core/Application.hpp"
#include "core/Events.hpp"

#include "utils/Logger.hpp"


namespace jdl
{
namespace core
{

// OpenGL debug callback
void APIENTRY s_DebugCallback(GLenum source, GLenum type, unsigned int id, GLenum severity,
                              GLsizei length, const char* message, const void* userParam)
{
    // Ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
    {
        return;
    }

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:
            JDL_ERROR(message);
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            JDL_WARN(message);
            break;
        case GL_DEBUG_SEVERITY_LOW:
            JDL_INFO(message);
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            JDL_DEBUG(message);
            break;
        default:
            break;
    }
}


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

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifndef NDEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif // !NDEBUG

    m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (m_window == nullptr)
    {
        JDL_FATAL("Failed to create the GLFW window");
    }
    glfwMakeContextCurrent(m_window);
    glfwSetWindowUserPointer(m_window, this);

    int gladStatus = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    if (!gladStatus)
    {
        JDL_FATAL("Failed to init GLAD");
    }

#ifndef NDEBUG
    int contextFlags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &contextFlags);

    if (contextFlags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

        glDebugMessageCallback(s_DebugCallback, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }
    else
    {
        JDL_WARN("Cannot setup the OpenGL debug context (not available)");
    }
#endif // !NDEBUG

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, width, height);

    setupCallbacks();
}

void Window::setupCallbacks()
{
    glfwSetWindowSizeCallback(
        m_window,
        [](GLFWwindow* window, int width, int height)
        {
            ResizeEvent event(width, height);
            Application::Get()->resizeEvent(event);
        }
    );
}

} // namespace core
} // namespace jdl
