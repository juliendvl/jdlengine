#include "core/Window.hpp"

#include "core/Application.hpp"

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

static Modifiers s_GetModifiers(GLFWwindow* window)
{
    Modifiers modifiers = Modifiers::kNone;

    // ALT
    auto left  = glfwGetKey(window, GLFW_KEY_LEFT_ALT);
    auto right = glfwGetKey(window, GLFW_KEY_RIGHT_ALT);
    if (left == GLFW_PRESS || right == GLFW_PRESS)
    {
        modifiers |= Modifiers::kAlt;
    }

    // CONTROL
    left  = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL);
    right = glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL);
    if (left == GLFW_PRESS || right == GLFW_PRESS)
    {
        modifiers |= Modifiers::kControl;
    }

    // SHIFT
    left  = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);
    right = glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT);
    if (left == GLFW_PRESS || right == GLFW_PRESS)
    {
        modifiers |= Modifiers::kShift;
    }

    return modifiers;
}

static std::pair<int, int> s_GetMousePosition(GLFWwindow* window)
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    return std::make_pair(static_cast<int>(xpos), static_cast<int>(ypos));
}


Window* Window::IWindow = nullptr;


Window::Window(const char* title, int width, int height)
    : m_window(nullptr)
    , m_pressedButton(MouseButton::kNone)
    , m_mouseX(0)
    , m_mouseY(0)
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
    glfwSetMouseButtonCallback(
        m_window,
        [](GLFWwindow* window, int button, int action, int mods)
        {
            auto appWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

            // Setup the mouse button
            MouseButton mouseButton = MouseButton::kNone;
            switch (button)
            {
                case GLFW_MOUSE_BUTTON_MIDDLE:
                    mouseButton = MouseButton::kMiddle;
                    break;
                case GLFW_MOUSE_BUTTON_RIGHT:
                    mouseButton = MouseButton::kRight;
                    break;
                default:
                    mouseButton = MouseButton::kLeft;
                    break;
            }

            // Setup the modifiers
            Modifiers modifiers = Modifiers::kNone;
            if (mods & GLFW_MOD_ALT)
                modifiers |= Modifiers::kAlt;
            if (mods & GLFW_MOD_CONTROL)
                modifiers |= Modifiers::kControl;
            if (mods & GLFW_MOD_SHIFT)
                modifiers |= Modifiers::kShift;

            auto mousePos = s_GetMousePosition(window);
            appWindow->m_mouseX = mousePos.first;
            appWindow->m_mouseY = mousePos.second;

            if (action == GLFW_PRESS)
            {
                appWindow->m_pressedButton = mouseButton;
                Application::Get().mousePressEvent(MousePressEvent(
                    mousePos.first, mousePos.second,
                    mouseButton,
                    modifiers
                ));
            }
            else
            {
                appWindow->m_pressedButton = MouseButton::kNone;
                Application::Get().mouseReleaseEvent(MouseReleaseEvent(
                    mousePos.first, mousePos.second,
                    mouseButton,
                    modifiers
                ));
            }
        }
    );

    glfwSetCursorPosCallback(
        m_window,
        [](GLFWwindow* window, double xpos, double ypos)
        {
            auto appWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

            int mouseX = static_cast<int>(xpos);
            int mouseY = static_cast<int>(ypos);
            int deltaX = mouseX - appWindow->m_mouseX;
            int deltaY = mouseY - appWindow->m_mouseY;

            Application::Get().mouseMoveEvent(MouseMoveEvent(
                mouseX, mouseY,
                deltaX, deltaY,
                appWindow->m_pressedButton,
                s_GetModifiers(window)
            ));

            appWindow->m_mouseX = mouseX;
            appWindow->m_mouseY = mouseY;
        }
    );

    glfwSetScrollCallback(
        m_window,
        [](GLFWwindow* window, double xoffset, double yoffset)
        {
            auto mousePos  = s_GetMousePosition(window);
            auto modifiers = s_GetModifiers(window);

            WheelEvent event(
                mousePos.first, mousePos.second,
                static_cast<int>(xoffset), static_cast<int>(yoffset),
                modifiers
            );
            Application::Get().wheelEvent(event);
        }
    );

    glfwSetWindowSizeCallback(
        m_window,
        [](GLFWwindow* window, int width, int height)
        {
            ResizeEvent event(width, height);
            Application::Get().resizeEvent(event);
        }
    );
}

} // namespace core
} // namespace jdl