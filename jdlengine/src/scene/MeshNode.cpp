#include "scene/MeshNode.hpp"


namespace jdl
{
namespace scene
{

MeshNode::MeshNode(const std::string& name)
    : SceneNode(name)
    , m_mesh(nullptr)
{}

MeshNode::MeshNode(const std::string& name, resource::Mesh* mesh)
    : SceneNode(name)
    , m_mesh(mesh)
{}

void MeshNode::doRender(core::RenderContext& context)
{
    if (m_mesh)
    {
        context.shaderProgram->setUniform("M", getWorldTransform().toMatrix());
        m_mesh->render();
    }
}

} // namespace scene
} // namespace jdl
