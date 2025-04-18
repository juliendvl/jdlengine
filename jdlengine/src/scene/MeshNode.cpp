#include "scene/MeshNode.hpp"


namespace jdl
{
namespace scene
{

MeshNode::MeshNode(const std::string& name, resource::RenderMesh* mesh)
    : SceneNode(name)
    , m_mesh(mesh)
{}

void MeshNode::doRender(core::RenderContext& context)
{
    if (m_mesh != nullptr)
    {
        m_mesh->render(context);
    }
}

} // namespace scene
} // namespace jdl
