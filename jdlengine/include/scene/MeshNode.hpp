#pragma once

#include "SceneNode.hpp"

#include "resource/Material.hpp"
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
     * @param name     Node name
     * @param mesh     Node mesh
     * @param material Optional material
     */
    MeshNode(const std::string& name, resource::Mesh* mesh, resource::Material* material = nullptr);

    /**
     * @brief Returns the mesh material if any.
     */
    resource::Material* getMaterial() const { return m_material; }

    /**
     * @brief Returns the rendered mesh.
     */
    resource::Mesh* getMesh() const { return m_mesh; }

    /**
     * @brief Sets the mesh material.
     * @param material The new material. Can be nullptr.
     */
    void setMaterial(resource::Material* material) { m_material = material; }

    /**
     * @brief Sets the rendered mesh.
     */
    void setMesh(resource::Mesh* mesh) { m_mesh = mesh; }

protected:
    virtual void doRender(core::RenderContext& context);

private:
    resource::Material* m_material;
    resource::Mesh* m_mesh;
};

} // namespace scene
} // namespace jdl
