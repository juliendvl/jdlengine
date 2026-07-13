#pragma once

#include "window.hpp"

#include "utils/non_copyable.hpp"


namespace jdl
{
namespace core
{

class Application : private NonCopyable<Application>
{
public:
    /**
     * @brief Creates the application.
     * @param name Application name
     * @param width Window initial width
     * @param height Window initial height
     */
    Application(const char* name, int width, int height);

    /**
     * @brief Destroys the application.
     */
    ~Application();

    /**
     * @brief Returns the created application instance.
     */
    static Application& Get() { return *s_Application; }

    /**
     * @brief Returns the application name.
     */
    static const char* GetName() { return s_Name; }

    /**
     * @brief Returns the application window.
     */
    static Window& GetWindow() { return *s_Application->m_window; }

    /**
     * @brief Runs the application.
     */
    void run();

private:
    static Application* s_Application;
    static const char* s_Name;

    std::unique_ptr<Window> m_window;
};

} // namespace core
} // namespace jdl
