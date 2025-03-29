#pragma once

#include "Resource.hpp"

#include "core/Buffers.hpp"
#include "core/Renderer.hpp"
#include "core/Vertex.hpp"


namespace jdl
{
namespace resource
{

class JDL_API RenderMesh : public Resource
{
public:
    /**
     * @brief Creates the mesh.
     * @param name Mesh name
     */
    RenderMesh(const std::string& name);

    /**
     * @brief Adds a new vertex.
     * @param vertex The new vertex
     */
    void addVertex(const core::Vertex& vertex);

    /**
     * @brief Adds multiple vertices.
     * @param start Vertex sequence start
     * @param end   Vertex sequence end
     */
    template<typename Iter>
    void addVertices(Iter start, Iter end)
    {
        m_vertices.insert(m_vertices.end(), start, end);
        m_verticesDirty = true;
    }

    /**
     * @brief Adds multiple vertices.
     * @param vertices The new vertices
     */
    void addVertices(const std::vector<core::Vertex>& vertices) {
        addVertices(vertices.cbegin(), vertices.cend());
    }

    /**
     * @brief Adds a new index.
     * @param index The new index
     */
    void addIndex(uint32_t index);

    /**
     * @brief Adds multiple indices.
     * @param start Index sequence start
     * @param end   Index sequence end
     */
    template<typename Iter>
    void addIndices(Iter start, Iter end)
    {
        m_indices.insert(m_indices.end(), start, end);
        m_indicesDirty = true;
    }

    /**
     * @brief Adds multiple indices.
     * @param indices The new indices
     */
    void addIndices(const std::vector<uint32_t>& indices) {
        addIndices(indices.cbegin(), indices.cend());
    }

    /**
     * @brief Returns the number of vertices.
     */
    size_t getNbVertices() const {
        return m_vertices.size();
    }

    /**
     * @brief Returns the number of indices.
     */
    size_t getNbIndices() const {
        return m_indices.size();
    }

    /**
     * @brief Renders the mesh.
     * @param context The render context
     */
    void render(core::RenderContext& context);

private:
    // Mesh data
    std::vector<core::Vertex> m_vertices;
    std::vector<uint32_t> m_indices;

    // Dirtiness flags
    bool m_verticesDirty = false;
    bool m_indicesDirty  = false;

    // Buffers
    std::unique_ptr<core::VertexBuffer> m_vertexBuffer;
    std::unique_ptr<core::IndexBuffer>  m_indexBuffer;

    // Updates the mesh buffers if necessary
    void updateBuffers();
    // Destroys the mesh data
    void clearResource();
};

} // namespace resource
} // namespace jdl
