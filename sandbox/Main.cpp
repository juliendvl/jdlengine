#include <core/Application.hpp>
#include <core/EntryPoint.hpp>
#include <core/Vertex.hpp>

#include <resource/RenderMesh.hpp>
#include <resource/ResourceManager.hpp>

#include <scene/MeshNode.hpp>
#include <scene/Scene.hpp>

#include <utils/Logger.hpp>

using namespace jdl;


class Sandbox : public core::Application
{
public:
    Sandbox(const char* name, int width, int height)
        : core::Application(name, width, height)
    {
        auto mesh = resource::ResourceManager::Create<resource::RenderMesh>("MESH");
        mesh->addVertices({
            {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
            {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
            {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
            {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}}
                            });
        mesh->addIndices({0, 1, 2, 0, 2, 3});

        auto node = scene::CreateNode<scene::MeshNode>("NODE", nullptr, mesh);

        auto scene = std::make_shared<scene::Scene>("SCENE");
        scene->setRootNode(node);

        GetRenderer().setScene(scene);
    }
};

std::unique_ptr<core::Application> CreateApplication(const char* name, int width, int height)
{
    return std::make_unique<Sandbox>(name, width, height);
}
