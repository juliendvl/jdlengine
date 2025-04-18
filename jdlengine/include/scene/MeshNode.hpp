#pragma once

#include "SceneNode.hpp"

#include "resource/RenderMesh.hpp"


namespace jdl
{
namespace scene
{

class JDL_API MeshNode : public SceneNode
{
public:
    /**
     * @brief Creates the node.
     * @param name Node name
     * @param mesh Node mesh (Optional)
     */
    MeshNode(const std::string& name, resource::RenderMesh* mesh = nullptr);

    /**
     * @brief Returns the node mesh.
     */
    resource::RenderMesh* getMesh() const { return m_mesh; }

    /**
     * @brief Sets the node mesh.
     */
    void setMesh(resource::RenderMesh* mesh) { m_mesh = mesh; }

private:
    resource::RenderMesh* m_mesh;

    void doRender(core::RenderContext& context);
};

} // namespace scene
} // namespace jdl
