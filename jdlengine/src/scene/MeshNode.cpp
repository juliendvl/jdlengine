#include "scene/MeshNode.hpp"


namespace jdl
{
namespace scene
{

MeshNode::MeshNode(const std::string& name) : SceneNode(name) {}

void MeshNode::doRender(core::RenderContext& context)
{
    if (m_mesh) {
        m_mesh->render(context);
    }
}

} // namespace scenes
} // namespace jdl
