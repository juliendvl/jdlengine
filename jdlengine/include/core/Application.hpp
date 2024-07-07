#pragma once

#include "Core.hpp"

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
     * @brief Runs the application.
     */
    void run();

private:
    // Application unique instance
    static Application* IApplication;
    // Application name
    const char* m_name;
};

std::unique_ptr<Application> CreateApplication(const char* name);

} // namespace core
} // namespace jdl
