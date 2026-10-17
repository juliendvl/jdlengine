#pragma once

#include <memory>

#include "window.hpp"

#include "utils/non_copyable.hpp"

#include "vk/vk_renderer.hpp"


namespace jdl
{
namespace core
{

class Application : private NonCopyable<Application>
{
    public:
        /**
         * @brief Creates the application.
         * @param name The application name.
         * @param width The application window initial width.
         * @param height The application window initial height.
         */
        Application(const char* name, int width, int height);

        /**
         * @brief Destroys the application.
         */
        ~Application();

        /**
         * @brief Returns the application instance.
         */
        static Application& Get() { return *s_Application; }

        /**
         * @brief Returns the application name.
         */
        static const char* GetName() { return s_Application->m_name; }

        /**
         * @brief Returns the application window.
         */
        static Window& GetWindow() { return *s_Application->m_window; }

        /**
         * @brief Returns the application renderer.
         */
        static vk::Renderer& GetRenderer() { return *s_Application->m_renderer; }

        /**
         * @brief Runs the application.
         */
        void run();

    private:
        static Application* s_Application;

        // Application name
        const char* m_name = nullptr;
        // Application window
        std::unique_ptr<Window> m_window;
        // Application renderer
        std::unique_ptr<vk::Renderer> m_renderer;
};

} // namespace core
} // namespace jdl
