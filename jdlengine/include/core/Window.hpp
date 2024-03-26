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
     * @param name   Window name.
     * @param width  Initial width.
     * @param height Initial height.
     */
    Window(const char* name, int width, int height);

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
     * @brief Returns whether the window is running or not.
     */
    bool isRunning() const;

    /**
     * @brief Processes the pending events and returns.
     */
    void pollEvents() const {
        glfwPollEvents();
    }

    /**
     * @brief Waits for events to be added to the event queue and processes them.
     */
    void waitEvents() const {
        glfwWaitEvents();
    }

    /**
     * @brief Swaps the window front and back buffers.
     */
    void swapBuffers() const {
        glfwSwapBuffers(m_window);
    }

private:
    // For now, only one window is supported
    static Window* IWindow;

    // GLFW window handle
    GLFWwindow* m_window;

    void setupCallbacks();
};

} // namespace core
} // namespace jdl
