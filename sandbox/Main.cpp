#include <core/Application.hpp>
#include <core/EntryPoint.hpp>

#include <math/SRTMatrix.hpp>

#include <resource/Cameras.hpp>
#include <resource/Mesh.hpp>
#include <resource/ResourceManager.hpp>

#include <scene/MeshNode.hpp>
#include <scene/Scene.hpp>

#include <utils/Logger.hpp>

using namespace jdl;


class Sandbox : public core::Application
{
public:
    Sandbox(const std::string& name, size_t width, size_t height)
        : core::Application(name, width, height)
    {
        auto mesh = resource::ResourceManager::Create<resource::Mesh>("MESH");
        mesh->addVertices({
            core::Vertex({0.0f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}),
            core::Vertex({0.5f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}),
            core::Vertex({-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f})
        });
        mesh->addIndices({0, 1, 2});

        auto node = scene::CreateNode<scene::MeshNode>("NODE", nullptr);
        node->setMesh(mesh);

        // math::SRTMatrix transform;
        // transform.scale = glm::vec3(0.5f);
        // transform.translation = glm::vec3(0.5f, 0.5f, 0.0f);
        // transform.setRotation(glm::vec3(0.0f, 0.0f, glm::radians(45.0f)));
        // node->setWorldTransform(transform);

        auto scene = std::make_shared<scene::Scene>("SCENE");
        scene->setRootNode(node);

        auto camera = static_cast<resource::PerspectiveCamera*>(scene->getCamera());
        camera->setAspectRatio(static_cast<float>(width) / height);

        GetRenderer().setScene(scene);
    }
};

std::unique_ptr<core::Application> core::CreateApplication(const core::ApplicationArgs& args)
{
    return std::make_unique<Sandbox>(args.name, args.width, args.height);
}
