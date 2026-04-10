#pragma once

#include "Window.hpp"

#include "utils/NonCopyable.hpp"

#include "vk/VulkanRenderer.hpp"


namespace jdl
{
namespace core
{

class Application : private NonCopyable<Application>
{
public:
	/**
	 * @brief Creates the application.
	 * @param name Application name.
	 * @param width Application initial width.
	 * @param height Application initial height.
	 */
	Application(const char* name, int width, int height);

	/**
	 * @brief Destroys the application.
	 */
	virtual ~Application();

	/**
	 * @brief Returns the application name.
	 */
	static const char* GetName() { return IName; }

	/**
	 * @brief Returns the application window.
	 */
	static Window& GetWindow() { return *IApplication->m_window; }

	/**
	 * @brief Returns the application renderer.
	 */
	static vk::VulkanRenderer& GetRenderer() {
		return *IApplication->m_renderer;
	}

	/**
	 * @brief Runs the application.
	 */
	void run();

private:
	static Application* IApplication;
	static const char* IName;

	std::unique_ptr<Window> m_window;
	std::unique_ptr<vk::VulkanRenderer> m_renderer;
};

} // namespace core
} // namespace jdl
