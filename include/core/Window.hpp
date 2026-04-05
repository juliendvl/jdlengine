#pragma once

#include "Size.hpp"

#include "utils/NonCopyable.hpp"

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
	 * @param title Window title.
	 * @param width Window initial width
	 * @param height Window initial height.
	 */
	Window(const char* name, int width, int height);

	/**
	 * @brief Destroys the window.
	 */
	~Window();

	/**
	 * @brief Returns the window size, in screen coordinates.
	 */
	Size getSize() const;

	/**
	 * @brief Returns the framebuffer size, in pixels.
	 */
	Size getFramebufferSize() const;

	/**
	 * @brief Returns whether the window is still running or not.
	 */
	bool isRunning() const;

	/**
	 * @brief Processes the events in the event queue and returns.
	 */
	void pollEvents() const { glfwPollEvents(); }

	/**
	 * @brief Waits for events to be added to the event queue and
	 * processes them.
	 */
	void waitEvents() const { glfwWaitEvents(); }

private:
	static Window* IWindow;

	GLFWwindow* m_window = nullptr;
};

} // namespace core
} // namespace jdl
