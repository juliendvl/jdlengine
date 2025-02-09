#include <core/Application.hpp>
#include <core/EntryPoint.hpp>

#include <utils/Logger.hpp>

using namespace jdl;


class Sandbox : public core::Application
{
public:
    Sandbox(const char* name, int width, int height)
        : core::Application(name, width, height)
    {}
};

std::unique_ptr<core::Application> CreateApplication(const char* name, int width, int height)
{
    return std::make_unique<Sandbox>(name, width, height);
}
