#pragma once

#include "SceneNode.hpp"

#include "math/BoundingBox.hpp"

#include "resource/Mesh.hpp"


namespace jdl
{
namespace scene
{

class JDL_API RenderableNode : public SceneNode
{
public:
    /**
     * @brief Creates the node.
     * @param name Node name
     */
    RenderableNode(const std::string& name);

    /**
     * @brief Adds a mesh to be rendered by the node.
     * @param mesh The new mesh
     */
    void addMesh(resource::Mesh* mesh) {
        m_meshes.push_back(mesh);
    }
    void addMesh(const std::string& mesh);

    /**
     * @brief Returns the number of meshes rendered by this node.
     */
    size_t getNbMeshes() const {
        return m_meshes.size();
    }

    /**
     * @brief Finds and returns one of the node rendered meshes.
     * @param index Mesh index
     */
    resource::Mesh* getMesh(size_t index) const;

    /**
     * @brief Removes a mesh from being rendered by the node.
     * @param index Mesh index
     */
    void removeMesh(size_t index);

private:
    // Rendered meshes
    std::vector<resource::Mesh*> m_meshes;

    math::BoundingBox getNodeBoundingBox() const;
    void doRender(const core::RenderContext& context);
};

} // namespace scene
} // namespace jdl
