#pragma once

#include <cassert>
#include <memory>
#include <string>

#include "window.hpp"

#include "utils/non_copyable.hpp"

#include "vk/vk_renderer.hpp"


namespace jdl
{
namespace core
{

class Application : private utils::NonCopyable<Application>
{
    public:
        /**
         * @brief Creates the application.
         * @param name The application name, copied by the application.
         * @param width The application window initial width.
         * @param height The application window initial height.
         */
        Application(std::string name, int width, int height);

        /**
         * @brief Destroys the application.
         */
        ~Application();

        /**
         * @brief Returns the application instance, which must exist.
         */
        static Application& Get()
        {
            assert(s_Application != nullptr && "The application has not been created");
            return *s_Application;
        }

        /**
         * @brief Returns the application name.
         */
        static const std::string& GetName() { return Get().m_name; }

        /**
         * @brief Returns the application window.
         */
        static Window& GetWindow() { return *Get().m_window; }

        /**
         * @brief Returns the application renderer.
         */
        static vk::Renderer& GetRenderer() { return *Get().m_renderer; }

        /**
         * @brief Runs the application.
         */
        void run();

    private:
        static Application* s_Application;

        // Application name
        std::string m_name;
        // Application window
        std::unique_ptr<Window> m_window;
        // Application renderer
        std::unique_ptr<vk::Renderer> m_renderer;
};

} // namespace core
} // namespace jdl
