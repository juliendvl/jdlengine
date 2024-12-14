#include "core/Window.hpp"

#include "utils/Logger.hpp"


namespace jdl
{
namespace core
{

static void APIENTRY s_DebugCallback(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length,
                                     const char* message, const void* userParam)
{
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:
            JDL_ERROR(message);
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            JDL_WARN(message);
            break;
        case GL_DEBUG_SEVERITY_LOW:
            JDL_DEBUG(message);
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            JDL_TRACE(message);
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
        JDL_FATAL("A Window already exists");
    }
    IWindow = this;

    glfwSetErrorCallback(
        [](int error, const char* description) {
            JDL_ERROR("GLFW {}: {}", error, description);
        }
    );

    int status = glfwInit();
    if (status == GLFW_FALSE)
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

    status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    if (status == 0)
    {
        JDL_FATAL("Failed to initialize GLAD");
    }
    glViewport(0, 0, width, height);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

#ifndef NDEBUG
    // Setup the debug context if available
    int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(s_DebugCallback, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }
    else
    {
        JDL_WARN("The OpenGL debug context cannot be enabled");
    }
#endif // !NDEBUG

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

void Window::setupCallbacks()
{
    glfwSetWindowSizeCallback(
        m_window,
        [](GLFWwindow* window, int width, int height)
        {
            glViewport(0, 0, width, height);
        }
    );
}

} // namespace core
} // namespace jdl