#include "scene/RenderableNode.hpp"

#include "resource/ResourceManager.hpp"

#include "utils/Logger.hpp"


namespace jdl
{
namespace scene
{

RenderableNode::RenderableNode(const std::string& name)
    : SceneNode(name)
{}

void RenderableNode::addMesh(const std::string& mesh)
{
    resource::Mesh* meshPtr = resource::ResourceManager::As<resource::Mesh>(mesh);
    if (meshPtr == nullptr)
    {
        JDL_ERROR("Mesh {} does not exist", mesh);
        return;
    }
    addMesh(meshPtr);
}

resource::Mesh* RenderableNode::getMesh(size_t index) const
{
    if (index >= getNbMeshes())
    {
        JDL_ERROR("Index {} is out of range", index);
        return nullptr;
    }
    return m_meshes[index];
}

void RenderableNode::removeMesh(size_t index)
{
    if (index < getNbMeshes())
    {
        m_meshes.erase(m_meshes.begin() + index);
    }
}

void RenderableNode::doRender(const core::RenderContext& context)
{
    context.shaderProgram->setUniform("M", getWorldTransform().toMatrix());
    for (resource::Mesh* mesh : m_meshes)
    {
        mesh->render(context);
    }
}

} // namespace scene
} // namespace jdl
