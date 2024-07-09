#include "core/Window.hpp"

#include "utils/Logger.hpp"


namespace jdl
{
namespace core
{

static void APIENTRY s_DebugCallback(GLenum source,
                                     GLenum type,
                                     unsigned int id,
                                     GLenum severity,
                                     GLsizei length,
                                     const char* message,
                                     const void* userParam)
{
    // Non-significant error and warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
    {
        return;
    }

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:
            JDL_ERROR("OpenGL - {}", message);
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            JDL_WARN("OpenGL - {}", message);
            break;
        case GL_DEBUG_SEVERITY_LOW:
            JDL_INFO("OpenGL - {}", message);
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            JDL_TRACE("OpenGL - {}", message);
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
        JDL_FATAL("Only one window can be created!");
    }
    IWindow = this;

    glfwSetErrorCallback(
        [](int error, const char* description) { JDL_ERROR("GLFW {} - {}", error, description); }
    );

    int status = glfwInit();
    if (status == GLFW_FALSE)
    {
        JDL_FATAL("Failed to initialize GLFW!");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifndef NDEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

    m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (m_window == nullptr)
    {
        JDL_FATAL("Failed to create the GLFW window!");
    }
    glfwMakeContextCurrent(m_window);

    status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    if (!status)
    {
        JDL_FATAL("Failed to initialize GLAD!");
    }

#ifndef NDEBUG
    // Check if the debug context is active or not
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
        JDL_WARN("The OpenGL debug context is not supported!");
    }
#endif

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, width, height);

    setupCallbacks();
}

Window::~Window()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

Size Window::getScreenSize() const
{
    Size outSize;
    glfwGetWindowSize(m_window, &outSize.width, &outSize.height);
    return outSize;
}

Size Window::getFramebufferSize() const
{
    Size outSize;
    glfwGetFramebufferSize(m_window, &outSize.width, &outSize.height);
    return outSize;
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
