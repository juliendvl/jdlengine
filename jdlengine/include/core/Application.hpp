#pragma once

#include "Core.hpp"

#include "utils/NonCopyable.hpp"


namespace core
{

class JDL_API Application : private NonCopyable<Application>
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
     * @brief Returns the application instance.
     */
    static Application& Get() { return *IApplication; }

    /**
     * @brief Returns the application name.
     */
    static const std::string& GetName() { return IApplication->m_name; }

    /**
     * @brief Runs the application.
     */
    void run();

private:
    // Unique Application instance
    static Application* IApplication;

    // Application name
    std::string m_name;
};

/*
 * @brief  Creates and returns the application.
 * @param  name Application name
 * @param  width Window initial width
 * @param  height Window initial height
 * @return The created application
 */
std::unique_ptr<Application> CreateApplication(const char* name, int width, int height);

} // namespace core
