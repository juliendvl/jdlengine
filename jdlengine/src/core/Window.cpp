#include "core/Window.hpp"

#include "core/Application.hpp"

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

static Modifiers s_GetModifiers(GLFWwindow* window)
{
    Modifiers modifiers = Modifiers::kNone;

    int lAltKey = glfwGetKey(window, GLFW_KEY_LEFT_ALT);
    int rAltKey = glfwGetKey(window, GLFW_KEY_RIGHT_ALT);
    if (lAltKey || rAltKey)
    {
        modifiers |= Modifiers::kAlt;
    }

    int lControlKey = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL);
    int rControlKey = glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL);
    if (lControlKey || rControlKey)
    {
        modifiers |= Modifiers::kControl;
    }

    int lShiftKey = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);
    int rShiftKey = glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT);
    if (lShiftKey || rShiftKey)
    {
        modifiers |= Modifiers::kShift;
    }

    return modifiers;
}


Window* Window::IWindow = nullptr;

Window::Window(const char* title, int width, int height)
    : m_window(nullptr)
    , m_pressButton(MouseButton::kNoButton)
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
    glfwSetWindowUserPointer(m_window, this);

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
    glfwSetMouseButtonCallback(
        m_window,
        [](GLFWwindow* window, int button, int action, int mods)
        {
            auto& application = Application::Get();
            Window* windowPtr = static_cast<Window*>(glfwGetWindowUserPointer(window));

            // Retrieve the mouse position
            double x, y;
            glfwGetCursorPos(window, &x, &y);

            // Retrieve the mouse button
            MouseButton mouseButton = MouseButton::kNoButton;
            switch (button)
            {
                case GLFW_MOUSE_BUTTON_LEFT:
                    mouseButton = MouseButton::kLeftButton;
                    break;
                case GLFW_MOUSE_BUTTON_RIGHT:
                    mouseButton = MouseButton::kRightButton;
                    break;
                case GLFW_MOUSE_BUTTON_MIDDLE:
                    mouseButton = MouseButton::kMiddleButton;
                    break;
                default:
                    mouseButton = MouseButton::kNoButton;
                    break;
            }

            // Retrieve the keyboard modifiers
            Modifiers modifiers = Modifiers::kNone;
            if (mods & GLFW_MOD_ALT)
            {
                modifiers |= Modifiers::kAlt;
            }
            if (mods & GLFW_MOD_CONTROL)
            {
                modifiers |= Modifiers::kControl;
            }
            if (mods & GLFW_MOD_SHIFT)
            {
                modifiers |= Modifiers::kShift;
            }

            if (action == GLFW_PRESS)
            {
                application.mousePressEvent(MousePressEvent(
                    static_cast<int>(x),
                    static_cast<int>(y),
                    mouseButton,
                    modifiers
                ));
                windowPtr->m_pressButton = mouseButton;
            }
            else
            {
                application.mouseReleaseEvent(MouseReleaseEvent(
                    static_cast<int>(x),
                    static_cast<int>(y),
                    mouseButton,
                    modifiers
                ));
                windowPtr->m_pressButton = MouseButton::kNoButton;
            }
        }
    );

    glfwSetCursorPosCallback(
        m_window,
        [](GLFWwindow* window, double xpos, double ypos)
        {
            Window* windowPtr = static_cast<Window*>(glfwGetWindowUserPointer(window));

            MouseMoveEvent event(
                static_cast<int>(xpos),
                static_cast<int>(ypos),
                windowPtr->m_pressButton,
                s_GetModifiers(window)
            );

            auto& application = Application::Get();
            application.mouseMoveEvent(event);
        }
    );

    glfwSetScrollCallback(
        m_window,
        [](GLFWwindow* window, double xoffset, double yoffset)
        {
            // Retrieve the mouse position
            double x, y;
            glfwGetCursorPos(window, &x, &y);

            WheelEvent event(
                static_cast<int>(x),
                static_cast<int>(y),
                static_cast<int>(yoffset),
                s_GetModifiers(window)
            );

            auto& application = Application::Get();
            application.wheelEvent(event);
        }
    );

    glfwSetWindowSizeCallback(
        m_window,
        [](GLFWwindow* window, int width, int height)
        {
            ResizeEvent event(width, height);

            auto& application = Application::Get();
            application.resizeEvent(event);
        }
    );
}

} // namespace core
} // namespace jdl
