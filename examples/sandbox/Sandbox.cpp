#include <JDLEngine.hpp>
#include <core/EntryPoint.hpp>

using namespace jdl;


class Sandbox : public core::Application
{
public:
    Sandbox(const char* name)
        : core::Application(name)
    {
        resource::Mesh* mesh = resource::ResourceManager::Create<resource::Mesh>("Mesh");
        mesh->addVertices({
            core::Vertex(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)),
            core::Vertex(glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)),
            core::Vertex(glm::vec3(0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)),
            core::Vertex(glm::vec3(-0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f))
        });
        mesh->addIndices({0, 1, 2, 0, 2, 3});

        core::Size windowSize = GetWindow().getFramebufferSize();

        resource::OrbitCamera* camera = resource::ResourceManager::Create<resource::OrbitCamera>("Camera");
        camera->setAspectRatio(static_cast<float>(windowSize.width) / windowSize.height);

        auto node = scene::CreateNode<scene::RenderableNode>("node", nullptr);
        node->addMesh(mesh);

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
