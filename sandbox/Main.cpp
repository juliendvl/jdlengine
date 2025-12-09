#include <core/Application.hpp>
#include <core/EntryPoint.hpp>

#include <resource/ResourceManager.hpp>
#include <resource/Shader.hpp>

#include <utils/Logger.hpp>

using namespace jdl;


class Sandbox : public core::Application
{
public:
    Sandbox(const std::string& name, size_t width, size_t height)
        : core::Application(name, width, height)
    {
        resource::ResourceManager::Create<resource::Shader>("default_vert", "shaders/default_vert.spv");
        resource::ResourceManager::Create<resource::Shader>("default_frag", "shaders/default_frag.spv");
    }
};

std::unique_ptr<core::Application> core::CreateApplication(const core::ApplicationArgs& args)
{
    return std::make_unique<Sandbox>(args.name, args.width, args.height);
}
