#pragma once

#include "size.hpp"

#include "utils/non_copyable.hpp"

#include <GLFW/glfw3.h>


namespace jdl
{
namespace core
{

class Window : private NonCopyable<Window>
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
     * @brief Returns the window size, in screen coordinates.
     */
    Size get_size() const;

    /**
     * @brief Returns the window framebuffer size, in pixels.
     */
    Size get_framebuffer_size() const;

    /**
     * @brief Returns whether the window is still running or not.
     */
    bool is_running() const {
        return glfwWindowShouldClose(m_window) == GLFW_FALSE;
    }

    /**
     * @brief Processes the events in the event queue and returns.
     */
    void poll_events() const {
        glfwPollEvents();
    }

    /**
     * @brief Waits for events to be added in the event queue and processes them.
     */
    void wait_events() const {
        glfwWaitEvents();
    }

    /**
     * @brief Returns the required Vulkan instance extensions for GLFW.
     */
    static std::vector<const char*> GetRequiredInstanceExtensions();

private:
    static Window* s_Window;

    GLFWwindow* m_window = nullptr;
};

} // namespace core
} // namespace jdl
