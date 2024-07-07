#include <JDLEngine.hpp>
#include <core/EntryPoint.hpp>


class Sandbox : public jdl::core::Application
{
public:
    Sandbox(const char* name)
        : jdl::core::Application(name)
    {
        std::cout << "Hello World!" << std::endl;
    }
};

std::unique_ptr<jdl::core::Application> CreateApplication(const char* name)
{
    return std::make_unique<Sandbox>(name);
}
