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
     * @param name Application name.
     */
    Application(const char* name);

    /**
     * @brief Destroys the application.
     */
    ~Application();

    /**
     * @brief Returns the application instance.
     */
    static Application& Get() {
        return *IApplication;
    }

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
    // Application unique instance
    static Application* IApplication;
    // Application name
    const char* m_name;
    // Application window
    std::unique_ptr<Window> m_window;
};

std::unique_ptr<Application> CreateApplication(const char* name);

} // namespace core
} // namespace jdl
