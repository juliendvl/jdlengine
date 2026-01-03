#pragma once

#include "Resource.hpp"

#include "core/RenderContext.hpp"
#include "core/Vertex.hpp"

#include "vulkan/VulkanBuffer.hpp"


namespace jdl
{
namespace resource
{

class Material;

class JDL_API Mesh : public Resource
{
public:
    /**
     * @brief Creates the mesh.
     * @param name The mesh name.
     */
    Mesh(const std::string& name);

    /**
     * @brief Adds a new vertex.
     * @param vertex The new vertex.
     */
    void addVertex(const core::Vertex& vertex);

    /**
     * @brief Adds multiple vertices.
     * @param start Vertex iterator start.
     * @param end   Vertex iterator end.
     */
    template<typename Iter>
    void addVertices(Iter start, Iter end)
    {
        m_vertices.insert(m_vertices.end(), start, end);
        m_reloadVertices = true;
    }

    /**
     * @brief Adds multiple vertices.
     * @param vertices The new vertices./
     */
    void addVertices(const std::vector<core::Vertex>& vertices) {
        addVertices(vertices.begin(), vertices.end());
    }

    /**
     * @brief Returns the number of vertices.
     */
    size_t getNbVertices() const {
        return m_vertices.size();
    }

    /**
     * @brief Adds a new index.
     * @param index The new index.
     */
    void addIndex(uint32_t index);

    /**
     * @brief Adds multiple indices.
     * @param start Index iterator start.
     * @param end   Index iterator end.
     */
    template<typename Iter>
    void addIndices(Iter start, Iter end)
    {
        m_indices.insert(m_indices.end(), start, end);
        m_reloadIndices = true;
    }

    /**
     * @brief Adds multiple indices.
     * @param vertices The new indices./
     */
    void addIndices(const std::vector<uint32_t>& indices) {
        addIndices(indices.begin(), indices.end());
    }

    /**
     * @brief Returns the number of indices.
     */
    size_t getNbIndices() const {
        return m_indices.size();
    }

    /**
     * @brief Returns the mesh material.
     */
    Material* getMaterial() const { return m_material; }

    /**
     * @brief Sets the mesh material.
     * @param material The new material.
     */
    void setMaterial(Material* material) { m_material = material; }

    /**
     * @brief Renders the mesh.
     * @param context Render context.
     */
    void render(core::RenderContext& context);

private:
    // Vertices
    std::vector<core::Vertex> m_vertices;
    bool m_reloadVertices = false;

    // Indices
    std::vector<uint32_t> m_indices;
    bool m_reloadIndices = false;

    // Buffers
    std::unique_ptr<vk::VulkanVertexBuffer> m_vertexBuffer = nullptr;
    std::unique_ptr<vk::VulkanIndexBuffer> m_indexBuffer = nullptr;

    // Material
    Material* m_material = nullptr;

    void updateBuffers();
    void clearResource();
};

} // namespace resource
} // namespace jdl
