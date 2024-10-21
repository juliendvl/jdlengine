#include <JDLEngine.hpp>
#include <core/EntryPoint.hpp>

using namespace jdl;


class Sandbox : public core::Application
{
public:
    Sandbox(const char* name)
        : core::Application(name)
    {

        core::Size windowSize = GetWindow().getFramebufferSize();

        auto node = io::ModelImporter::Import("resources/models/BoxTextured/BoxTextured.gltf");
        auto bbox = node->getBoundingBox();

        resource::OrbitCamera* camera = resource::ResourceManager::Create<resource::OrbitCamera>("Camera");
        camera->setAspectRatio(static_cast<float>(windowSize.width) / windowSize.height);
        camera->setCenter(bbox.getCenter());
        camera->setRadius(2.0f * glm::max(bbox.getWidth(), bbox.getHeight(), bbox.getDepth()));

        auto scene = std::make_shared<scene::Scene>("scene");
        scene->setCamera(camera);
        scene->setRootNode(node);

        GetRenderer().setScene(scene);
    }
};

std::unique_ptr<core::Application> CreateApplication(const char* name)
{
    return std::make_unique<Sandbox>(name);
}
