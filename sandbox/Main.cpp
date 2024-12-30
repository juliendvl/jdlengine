#include <JDLEngine.hpp>
#include <core/EntryPoint.hpp>

using namespace jdl;


class Sandbox : public core::Application
{
public:
    Sandbox(const char* name, int width, int height)
        : core::Application(name, width, height)
    {
        auto mesh = resource::ResourceManager::Create<resource::Mesh>("MESH");
        mesh->addVertices({
            core::Vertex(glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.5f, 1.0f)),
            core::Vertex(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)),
            core::Vertex(glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f))
        });
        mesh->addIndices({0, 1, 2});

        auto meshNode = scene::CreateNode<scene::MeshNode>("MESH_NODE", nullptr, mesh);

        auto scene = std::make_shared<scene::Scene>("SCENE");
        scene->setRootNode(meshNode);

        GetRenderer().setScene(scene);
    }
};


std::unique_ptr<core::Application> CreateApplication(const char* name, int width, int height)
{
    return std::make_unique<Sandbox>(name, width, height);
}
