#pragma once

#include "Core.hpp"
#include "Window.hpp"

#include "utils/NonCopyable.hpp"


namespace jdl
{
namespace core
{

class JDL_API Application : private NonCopyable<Application>
{
public:
    /**
     * @brief Creates the application.
     * @param name   Application name
     * @param width  Initial window width
     * @param height Initial window height
     */
    Application(const char* name, int width, int height);

    /**
     * @brief Destroys the application.
     */
    ~Application();

    /**
     * @brief Returns the unique Application instance.
     */
    static Application* Get() { return IApplication; }

    /**
     * @brief Returns the application name.
     */
    static const char* GetName() { return IApplication->m_name; }

    /**
     * @brief Returns the application window.
     */
    static Window& GetWindow() { return *IApplication->m_window; }

    /**
     * @brief Runs the application.
     */
    void run();

private:
    // Unique application instance
    static Application* IApplication;

    // Application window
    std::unique_ptr<Window> m_window;

    const char* m_name;
};

/**
 * @brief  Creates and returns the application.
 * @param  name   Application name
 * @param  width  Initial window width
 * @param  height Initial window height
 * @return Application
 */
std::unique_ptr<Application> CreateApplication(const char* name, int width, int height);

} // namespace core
} // namespace jdl
