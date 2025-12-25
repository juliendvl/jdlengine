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
     * @param name The node name.
     */
    MeshNode(const std::string& name);

    /**
     * @brief Returns the node rendered mesh.
     */
    resource::Mesh* getMesh() const {
        return m_mesh;
    }

    /**
     * @brief Sets the node mesh to be rendered.
     * @param mesh The new mesh.
     */
    void setMesh(resource::Mesh* mesh) {
        m_mesh = mesh;
    }

private:
    resource::Mesh* m_mesh = nullptr;

    void doRender(core::RenderContext& context);
};

} // namespace scene
} // namespace jdl
