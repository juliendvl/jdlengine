#pragma once

#include "Core.hpp"
#include "Events.hpp"
#include "Renderer.hpp"
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
     * @brief Returns the renderer object.
     */
    static Renderer& GetRenderer() {
        return *IApplication->m_renderer;
    }

    /**
     * @brief Runs the application.
     */
    void run();

    /**
     * @brief Resize event.
     * @param event Event data.
     */
    void resizeEvent(const ResizeEvent& event);

private:
    // Application unique instance
    static Application* IApplication;
    // Application name
    const char* m_name;
    // Application window
    std::unique_ptr<Window> m_window;
    // Renderer
    std::unique_ptr<Renderer> m_renderer;
};

std::unique_ptr<Application> CreateApplication(const char* name);

} // namespace core
} // namespace jdl
