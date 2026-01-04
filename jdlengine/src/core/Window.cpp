#include "core/Window.hpp"
#include "core/Events.hpp"
#include "core/Application.hpp"

#include "utils/Logger.hpp"


namespace jdl
{
namespace core
{

static Modifiers s_GetModifiers(GLFWwindow* window)
{
    Modifiers modifiers = Modifiers::eNone;

    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) {
        modifiers |= Modifiers::eAlt;
    }
    else if (glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS) {
        modifiers |= Modifiers::eAlt;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        modifiers |= Modifiers::eControl;
    }
    else if (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS) {
        modifiers |= Modifiers::eControl;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        modifiers |= Modifiers::eShift;
    }
    else if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) {
        modifiers |= Modifiers::eShift;
    }

    return modifiers;
}

static MouseButton s_GetMouseButton(GLFWwindow* window)
{
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        return MouseButton::eLeftButton;
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        return MouseButton::eRightButton;
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) {
        return MouseButton::eMiddleButton;
    }

    return MouseButton::eNone;
}

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
#ifndef _WIN32
    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
#endif // _WIN32

    int status = glfwInit();
    if (status == GLFW_FALSE) {
        JDL_FATAL("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    m_window = glfwCreateWindow((int)width, (int)height, title.c_str(), nullptr, nullptr);
    if (m_window == nullptr) {
        JDL_FATAL("Failed to create the GLFW window");
    }
    glfwSetWindowUserPointer(m_window, this);

    setupCallbacks();
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

void Window::setupCallbacks()
{
    // Resize event
    glfwSetWindowSizeCallback(
        m_window,
        [](GLFWwindow* window, int width, int height)
        {
            ResizeEvent event(width, height);
            Application::Get().resizeEvent(event);
        }
    );

    // Mouse press/release callback
    glfwSetMouseButtonCallback(
        m_window,
        [](GLFWwindow* window, int button, int action, int mods)
        {
            Window* winHandle = static_cast<Window*>(glfwGetWindowUserPointer(window));

            // Mouse button
            MouseButton mouseButton = MouseButton::eNone;
            if (button == GLFW_MOUSE_BUTTON_LEFT) {
                mouseButton = MouseButton::eLeftButton;
            }
            else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
                mouseButton = MouseButton::eRightButton;
            }
            else if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
                mouseButton = MouseButton::eMiddleButton;
            }

            // Keyboard modifiers
            Modifiers modifiers = Modifiers::eNone;
            if (mods & GLFW_MOD_ALT) {
                modifiers |= Modifiers::eAlt;
            }
            if (mods & GLFW_MOD_CONTROL) {
                modifiers |= Modifiers::eControl;
            }
            if (mods & GLFW_MOD_SHIFT) {
                modifiers |= Modifiers::eShift;
            }

            // Mouse position
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);

            winHandle->m_lastX = static_cast<int>(xpos);
            winHandle->m_lastY = static_cast<int>(ypos);

            if (action == GLFW_PRESS)
            {
                MousePressEvent event(mouseButton, winHandle->m_lastX, winHandle->m_lastY, modifiers);
                Application::Get().mousePressEvent(event);
            }
            else
            {
                MouseReleaseEvent event(mouseButton, winHandle->m_lastX, winHandle->m_lastY, modifiers);
                Application::Get().mouseReleaseEvent(event);
            }
        }
    );

    // Mouse move event
    glfwSetCursorPosCallback(
        m_window,
        [](GLFWwindow* window, double xpos, double ypos)
        {
            Window* winHandle = static_cast<Window*>(glfwGetWindowUserPointer(window));

            int mouseX = static_cast<int>(xpos);
            int mouseY = static_cast<int>(ypos);

            MouseMoveEvent event(
                s_GetMouseButton(window),
                mouseX, mouseY,
                mouseX - winHandle->m_lastX, mouseY - winHandle->m_lastY,
                s_GetModifiers(window)
            );
            Application::Get().mouseMoveEvent(event);

            winHandle->m_lastX = mouseX;
            winHandle->m_lastY = mouseY;
        }
    );

    // Mouse wheel event
    glfwSetScrollCallback(
        m_window,
        [](GLFWwindow* window, double xoffset, double yoffset)
        {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);

            WheelEvent event(
                static_cast<int>(xpos), static_cast<int>(ypos),
                static_cast<int>(xoffset), static_cast<int>(yoffset),
                s_GetModifiers(window)
            );
            Application::Get().wheelEvent(event);
        }
    );
}

} // namespace core
} // namespace jdl
