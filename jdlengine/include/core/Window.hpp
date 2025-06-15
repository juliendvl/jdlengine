#pragma once

#include "Core.hpp"
#include "Size.hpp"

#include <GLFW/glfw3.h>

#include "utils/NonCopyable.hpp"


namespace core
{

class JDL_API Window : private NonCopyable<Window>
{
public:
    /**
     * @brief Creates the window.
     * @param title Window title
     * @param width Initial width
     * @param height Initial height
     */
    Window(const char* title, int width, int height);

    /**
     * @brief Destroys the window.
     */
    ~Window();

    /**
     * @brief Returns the window screen size.
     */
    Size getScreenSize() const;

    /**
     * @brief Returns the window framebuffer size.
     */
    Size getFramebufferSize() const;

    /**
     * @brief Returns whether the closing flag has been set for the window.
     */
    bool isClosing() const {
        return glfwWindowShouldClose(m_window) == GLFW_TRUE;
    }

    /**
     * @brief Processes the events in the event queue and returns.
     */
    void pollEvents() const {
        glfwPollEvents();
    }

    /**
     * @brief Waits for events to be added to the event queue and processes them.
     */
    void waitEvents() const {
        return glfwWaitEvents();
    }

    /**
     * @brief Returns the window required Vulkan instance extensions.
     */
    static std::vector<const char*> GetRequiredInstanceExtensions();

private:
    // Unique Window instance
    static Window* IWindow;
    // GLFW Pointer
    GLFWwindow* m_window;
};

} // namespace core
