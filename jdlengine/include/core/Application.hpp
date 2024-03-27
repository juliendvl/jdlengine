#pragma once

#include "Core.hpp"
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
     * @param name   Application name.
     * @param width  Application initial width.
     * @param height Application initial height.
     */
    Application(const char* name, int width, int height);

    /**
     * @brief Safely destroys the application.
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
        return Get().m_name;
    }

    /**
     * @brief Returns the application window.
     */
    static Window& GetWindow() {
        return *Get().m_window;
    }

    /**
     * @brief Returns the application renderer.
     */
    static Renderer& GetRenderer() {
        return *Get().m_renderer;
    }

    /**
     * @brief Runs the application.
     */
    void run();

private:
    // Unique application instance
    static Application* IApplication;

    // Application name
    const char* m_name;

    // Application window
    std::unique_ptr<Window> m_window;
    // Application renderer
    std::unique_ptr<Renderer> m_renderer;

    // Setups the OpenGL debug context
    void setupDebugContext();
};

/**
 * @brief Creates and returns an application. This has to be defined by the library user.
 * @param name   Application name.
 * @param width  Application initial width.
 * @param height Application initial height.
 */
std::unique_ptr<Application> CreateApplication(const char* name, int width, int height);

} // namespace core
} // namespace jdl
