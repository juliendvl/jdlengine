#pragma once

#include "Core.hpp"
#include "Size.hpp"

#include "utils/NonCopyable.hpp"

#include <GLFW/glfw3.h>


namespace jdl
{
namespace core
{

class JDL_API Window : private NonCopyable<Window>
{
public:
    /**
     * @brief Creates the window.
     * @param title  Window title.
     * @param width  Window initial width
     * @param height Window initial height.
     */
    Window(const char* title, int width, int height);

    /**
     * @brief Destroys the window.
     */
    ~Window();

    /**
     * @brief Processes the events in the event queue and returns.
     */
    void pollEvents() const {
        glfwPollEvents();
    }

    /**
     * @brief Waits for events to be queued in the event queue and processes them.
     */
    void waitEvents() const {
        glfwWaitEvents();
    }

    /**
     * @brief Returns whether the window close flag has been triggered or not.
     */
    bool shouldClose() const {
        return glfwWindowShouldClose(m_window) == GLFW_TRUE;
    }

    /**
     * @brief Swaps the window front and back buffers.
     */
    void swapBuffers() const {
        glfwSwapBuffers(m_window);
    }

    /**
     * @brief Returns the window size in screen coordinates.
     */
    Size getScreenSize() const;

    /**
     * @brief Returns the window framebuffer size in pixels.
     */
    Size getFramebufferSize() const;

private:
    static Window* IWindow;

    // Internal window handle
    GLFWwindow* m_window;

    void setupCallbacks();
};

} // namespace core
} // namespace jdl
