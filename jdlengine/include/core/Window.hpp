#pragma once

#include "Core.hpp"
#include "utils/NonCopyable.hpp"

// GLAD must be included before GLFW
#include <GLFW/glfw3.h>

#include "Size.hpp"


namespace jdl
{
namespace core
{

class JDL_API Window : private utils::NonCopyable<Window>
{
public:
    /** 
     * @brief Creates the application window.
     * @param title  Window title
     * @param width  Initial width
     * @param height Initial height
     */
    Window(const char* title, int width, int height);

    /**
     * @brief Safely destroys the window.
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
     * @brief Processes the pending events and returns.
     */
    void pollEvents() const { glfwPollEvents(); }

    /**
     * @brief Waits for events to be queued and processes them.
     */
    void waitEvents() const { glfwWaitEvents(); }

    /**
     * @brief Returns whether the window is still running or not.
     */
    bool isRunning() const {
        return glfwWindowShouldClose(m_window) == GLFW_FALSE;
    }

    /**
     * @brief Swaps the window front and back buffers.
     */
    void swapBuffers() const {
        glfwSwapBuffers(m_window);
    }

private:
    // Single Window instance
    static Window* IWindow;

    // Internal GLFW pointer
    GLFWwindow* m_window;

    // Setups the window callbacks (mouse, keyboard, resize...)
    void setupCallbacks();
};

} // namespace core
} // namespace jdl
