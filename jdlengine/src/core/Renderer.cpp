#include "core/Renderer.hpp"

// TODO - To be removed!
#include "core/Vertex.hpp"
#include "resource/Mesh.hpp"


namespace jdl
{
namespace core
{

static resource::Mesh* sMesh = nullptr;

Renderer::Renderer()
{
    sMesh = resource::ResourceManager::Create<resource::Mesh>("Mesh");
    sMesh->addVertices({
        core::Vertex(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)),
        core::Vertex(glm::vec3( 0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)),
        core::Vertex(glm::vec3( 0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)),
        core::Vertex(glm::vec3(-0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f))
    });
    sMesh->addIndices({0, 1, 2, 0, 2, 3});
}

Renderer::~Renderer() {}

void Renderer::renderFrame()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    sMesh->render();
}

void Renderer::resizeEvent(const ResizeEvent& event)
{
    glViewport(0, 0, event.getWidth(), event.getHeight());
}

} // namespace core
} // namespace jdl
