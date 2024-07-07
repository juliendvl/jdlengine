#include <JDLEngine.hpp>
#include <core/EntryPoint.hpp>


class Sandbox : public jdl::core::Application
{
public:
    Sandbox(const char* name)
        : jdl::core::Application(name)
    {
        JDL_TRACE("TRACE Message");
        JDL_DEBUG("DEBUG Message");
        JDL_INFO("INFO Message");
        JDL_WARN("WARN Message");
        JDL_ERROR("ERROR Message");
    }
};

std::unique_ptr<jdl::core::Application> CreateApplication(const char* name)
{
    return std::make_unique<Sandbox>(name);
}
