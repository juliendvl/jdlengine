#pragma once

#include "Core.hpp"
#include "utils/NonCopyable.hpp"

#include "Events.hpp"
#include "Renderer.hpp"
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
     * @brief Returns the application instance.
     */
    static Application& Get() { return *IApplication; }

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
     * @brief Returns the application renderer.
     */
    static Renderer& GetRenderer() {
        return *IApplication->m_renderer;
    }

    /**
     * @brief Runs the application.
     */
    void run();

    /**
     * @brief Resize event callback.
     * @param event Event data
     */
    void resizeEvent(const ResizeEvent& event);

private:
    // Singleton Instance
    static Application* IApplication;

    // Application name
    const char* m_name;
    // Application window
    std::unique_ptr<Window> m_window;
    // Application renderer
    std::unique_ptr<Renderer> m_renderer;
};

std::unique_ptr<Application> CreateApplication(const char* name, int width, int height);

} // namespace core
} // namespace jdl
