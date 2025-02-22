#pragma once

#include "Core.hpp"
#include "Size.hpp"

#include <GLFW/glfw3.h>

#include "utils/NonCopyable.hpp"


namespace jdl
{
namespace core
{

class JDL_API Window : private NonCopyable<Window>
{
public:
    /**
     * @brief Creates the window.
     * @param title  Window title
     * @param width  Initial width
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
     * @brief Processes the events in the events queue and returns.
     */
    void pollEvents() const { glfwPollEvents(); }

    /**
     * @brief Waits for events to be added to the events queue and processes them.
     */
    void waitEvents() const { glfwWaitEvents(); }

    /**
     * @brief Returns whether the window is running or not.
     */
    bool isRunning() const { return glfwWindowShouldClose(m_window) == GLFW_FALSE; }

    /**
     * @brief Returns the Vulkan instance extensions needed to interface with the window system.
     */
    static std::vector<const char*> GetInstanceExtensions();

private:
    // Unique window instance
    static Window* IWindow;

    // GLFW handle
    GLFWwindow* m_window;

    // Setups and inits the window
    void init(const char* title, int width, int height);
    // Setups the window callbacks
    void setupCallbacks();
};

} // namespace core
} // namespace jdl
