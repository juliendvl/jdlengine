#include "scene/MeshNode.hpp"

#include <glm/gtc/type_ptr.hpp>


namespace jdl
{
namespace scene
{

MeshNode::MeshNode(const std::string& name) : SceneNode(name) {}

void MeshNode::doRender(core::RenderContext& context)
{
    if (!m_mesh) {
        return;
    }

    // Send the model matrix using a push constant
    glm::mat4 M = getWorldTransform().toMatrix();
    vkCmdPushConstants(
        context.commandBuffer, context.pipelineLayout,
        VK_SHADER_STAGE_VERTEX_BIT,
        0, sizeof(glm::mat4), glm::value_ptr(M)
    );
    m_mesh->render(context);
}

} // namespace scenes
} // namespace jdl
