#include <JDLEngine.hpp>

#include <core/Application.hpp>
#include <core/EntryPoint.hpp>

using namespace jdl;


class Sandbox : public core::Application
{
public:
    Sandbox(const char* name, int width, int height)
        : core::Application(name, width, height)
    {
        utils::Logger::SetLevel(utils::LogLevel::kTrace);

        JDL_TRACE("TRACE Message");
        JDL_DEBUG("DEBUG Message");
        JDL_INFO("INFO Message");
        JDL_WARN("WARN Message");
        JDL_ERROR("ERROR Message");
    }
};

std::unique_ptr<core::Application> CreateApplication(const char* name, int width, int height)
{
    return std::make_unique<Sandbox>(name, width, height);
}
