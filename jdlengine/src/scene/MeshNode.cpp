#include "scene/MeshNode.hpp"


namespace jdl
{
namespace scene
{

MeshNode::MeshNode(const std::string& name)
    : SceneNode(name)
    , m_mesh(nullptr)
    , m_material(nullptr)
{}

MeshNode::MeshNode(const std::string& name, resource::Mesh* mesh, resource::Material* material)
    : SceneNode(name)
    , m_mesh(mesh)
    , m_material(material)
{}

void MeshNode::doRender(core::RenderContext& context)
{
    if (m_mesh)
    {
        if (m_material)
        {
            m_material->bind(context.shaderProgram);
        }

        context.shaderProgram->setUniform("M", getWorldTransform().toMatrix());
        m_mesh->render();
    }
}

} // namespace scene
} // namespace jdl
