#pragma once

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
        const char* GetName() { return s_Application->m_name; }

        /**
         * @brief Runs the application.
         */
        void run();

    private:
        static Application* s_Application;

        const char* m_name = nullptr;
};

} // namespace core
} // namespace jdl
