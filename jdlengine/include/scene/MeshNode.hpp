#pragma once

#include "SceneNode.hpp"

#include "resource/Mesh.hpp"


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
     */
    MeshNode(const std::string& name);

    /**
     * @brief Creates the node.
     * @param name Node name
     * @param mesh Node mesh
     */
    MeshNode(const std::string& name, resource::Mesh* mesh);

    /**
     * @brief Returns the rendered mesh.
     */
    resource::Mesh* getMesh() const { return m_mesh; }

    /**
     * @brief Sets the rendered mesh.
     */
    void setMesh(resource::Mesh* mesh) { m_mesh = mesh; }

protected:
    virtual void doRender(core::RenderContext& context);

private:
    resource::Mesh* m_mesh;
};

} // namespace scene
} // namespace jdl
