#pragma once

#include <vector>

#include <vulkan/vulkan.h>

#include "size.hpp"

#include "utils/non_copyable.hpp"

// GLFW stays an implementation detail of the engine: only window.cpp includes it.
struct GLFWwindow;

namespace jdl
{
namespace core
{

class Window : private utils::NonCopyable<Window>
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
	bool is_running() const;

	/**
	 * @brief Processes pending events without blocking.
	 */
	void poll_events() const;

	/**
	 * @brief Blocks until at least one event is available, then processes
	 * pending events.
	 */
	void wait_events() const;

	/**
	 * @brief Creates a Vulkan surface for the window.
	 * @param instance The Vulkan instance the surface is created for.
	 * @return The surface handle, to be destroyed by the caller before the instance.
	 */
	VkSurfaceKHR create_surface(VkInstance instance) const;

	/**
	 * @brief Returns the Vulkan instance extensions required by GLFW.
	 */
	static std::vector<const char*> GetInstanceExtensions();
	
private:
	static Window* s_Window;

	GLFWwindow* m_window = nullptr;
};

} // namespace core
} // namespace jdl
