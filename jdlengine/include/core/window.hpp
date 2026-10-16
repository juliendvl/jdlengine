#pragma once

#include <GLFW/glfw3.h>

#include "size.hpp"

#include "utils/non_copyable.hpp"


namespace jdl
{
namespace core
{

class Window : private NonCopyable<Window>
{
public:
	/**
	 * @brief Creates the window and initializes GLFW.
	 * @param title The window title.
	 * @param width The window initial width.
	 * @param height The window initial height.
	 */
	Window(const char* title, int width, int height);

	/**
	 * @brief Destroys the window and terminates GLFW.
	 */
	~Window();

	/**
	 * @brief Returns the window size in screen coordinates.
	 */
	Size screen_size() const;

	/**
	 * @brief Returns the framebuffer size in pixels.
	 */
	Size framebuffer_size() const;

	/**
	 * @brief Returns whether the window has not been requested to close.
	 */
	bool is_running() const {
		return glfwWindowShouldClose(m_window) == GLFW_FALSE;
	}

	/**
	 * @brief Processes pending events without blocking.
	 */
	void poll_events() const { glfwPollEvents(); }

	/**
	 * @brief Blocks until at least one event is available, then processes
	 * pending events.
	 */
	void wait_events() const { glfwWaitEvents(); }
	
private:
	static Window* s_Window;

	GLFWwindow* m_window = nullptr;
};

} // namespace core
} // namespace jdl
