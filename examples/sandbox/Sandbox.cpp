#include <JDLEngine.hpp>
#include <core/EntryPoint.hpp>

using namespace jdl;


class Sandbox : public core::Application
{
public:
    Sandbox(const char* name)
        : core::Application(name)
    {}
};

std::unique_ptr<core::Application> CreateApplication(const char* name)
{
    return std::make_unique<Sandbox>(name);
}
