#include "core/Renderer.hpp"

#include "core/Vertex.hpp"
#include "resource/Mesh.hpp"


namespace jdl
{
namespace core
{

static resource::Mesh* MESH = nullptr;

Renderer::Renderer()
{
    MESH = resource::ResourceManager::Create<resource::Mesh>("MESH");
    MESH->addVertices({
        core::Vertex(glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.5f, 1.0f)),
        core::Vertex(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)),
        core::Vertex(glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f))
    });
    MESH->addIndices({0, 1, 2});

    m_context.shaderProgram = resource::ResourceManager::As<resource::ShaderProgram>("DefaultShader");
}

Renderer::~Renderer() {}

void Renderer::renderFrame()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    if (m_scene != nullptr)
    {
        m_scene->render(m_context);
    }
    MESH->render();
}

void Renderer::resizeEvent(const ResizeEvent& event)
{
    glViewport(0, 0, event.getWidth(), event.getHeight());
    if (m_scene != nullptr)
    {
        m_scene->resizeEvent(event);
    }
}

} // namespace core
} // namespace jdl
