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
     * 
     * @param title Window title.
     * @param width Initial width.
     * @param height Initial height.
     */
    Window(const std::string& title, size_t width, size_t height);

    /**
     * @brief Destroys the window.
     */
    ~Window();

    /**
     * @brief Returns the window instance.
     */
    static Window& Get() {
        return *IWindow;
    }

    /**
     * @brief Returns the GLFW window handle.
     */
    GLFWwindow* getHandle() const {
        return m_window;
    }

    /**
     * @brief Returns the window size, in screen coordinates.
     */
    Size getSize() const;

    /**
     * @brief Returns the window framebuffer size.
     */
    Size getFramebufferSize() const;

    /**
     * @brief Returns whether the window is still running or not (i.e. the window close flag
     * has not been triggered).
     */
    bool isRunning() const {
        return glfwWindowShouldClose(m_window) == GLFW_FALSE;
    }

    /**
     * @brief Processes the events in the event queue and returns.
     */
    void pollEvents() const {
        glfwPollEvents();
    }

    /**
     * @brief Waits for events to be added in the event queue and processes them.
     */
    void waitEvents() const {
        glfwWaitEvents();
    }

    /**
     * @brief Returns all the instance extensions that are required to work with the GLFW window.
     */
    static std::vector<const char*> GetInstanceExtensions();

private:
    // Single Window instance
    static Window* IWindow;
    // Internal window pointer
    GLFWwindow* m_window = nullptr;
};

} // namespace core
} // namespace jdl
