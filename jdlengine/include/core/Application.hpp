#pragma once

#include "Core.hpp"
#include "utils/NonCopyable.hpp"

#include "Window.hpp"


namespace jdl
{
namespace core
{

class JDL_API Application : private utils::NonCopyable<Application>
{
public:
    /**
     * @brief Creates the application.
     * @param name   Application name
     * @param width  Initial width
     * @param height Initial height
     */
    Application(const char* name, int width, int height);

    /**
     * @brief Safely destroys the application.
     */
    ~Application();

    /**
     * @brief Returns the application name.
     */
    static const char* GetName() {
        return IApplication->m_name;
    }

    /**
     * @brief Returns the application window.
     */
    static Window& GetWindow() {
        return *IApplication->m_window;
    }

    /**
     * @brief Runs the application.
     */
    void run();

private:
    // Singleton Instance
    static Application* IApplication;

    // Application name
    const char* m_name;
    // Application window
    std::unique_ptr<Window> m_window;
};

std::unique_ptr<Application> CreateApplication(const char* name, int width, int height);

} // namespace core
} // namespace jdl
