#include <JDLEngine.hpp>
#include <core/EntryPoint.hpp>

using namespace jdl;


class Sandbox : public core::Application
{
public:
    Sandbox(const char* name, int width, int height)
        : core::Application(name, width, height)
    {
        auto size = GetWindow().getFramebufferSize();

        auto texture = resource::ResourceManager::Create<resource::Texture>("TEXTURE", "textures/container.png");

        auto material = resource::ResourceManager::Create<resource::Material>("MATERIAL");
        material->setBaseColorTexture(texture);

        auto camera = resource::ResourceManager::Create<resource::OrbitCamera>("CAMERA");
        camera->setAspectRatio(static_cast<float>(size.width) / size.height);
        camera->setAzimuth(glm::radians(90.0f));

        auto mesh = resource::ResourceManager::Create<resource::Mesh>("MESH");
        mesh->addVertices({
            core::Vertex(glm::vec3(-0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)),
            core::Vertex(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)),
            core::Vertex(glm::vec3( 0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)),
            core::Vertex(glm::vec3( 0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f))
        });
        mesh->addIndices({0, 1, 2, 0, 2, 3});

        auto meshNode = scene::CreateNode<scene::MeshNode>("MESH_NODE", nullptr, mesh, material);

        auto scene = std::make_shared<scene::Scene>("SCENE");
        scene->setRootNode(meshNode);
        scene->setCamera(camera);

        GetRenderer().setScene(scene);
    }
};


std::unique_ptr<core::Application> CreateApplication(const char* name, int width, int height)
{
    return std::make_unique<Sandbox>(name, width, height);
}
