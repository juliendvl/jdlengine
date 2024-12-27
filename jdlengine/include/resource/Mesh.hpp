#pragma once

#include "Resource.hpp"

#include "core/Vertex.hpp"


namespace jdl
{
namespace resource
{

enum class PrimitiveType
{
    kPoints        = GL_POINTS,
    kLines         = GL_LINES,
    kLineStrip     = GL_LINE_STRIP,
    kLineLoop      = GL_LINE_LOOP,
    kTriangles     = GL_TRIANGLES,
    kTriangleStrip = GL_TRIANGLE_STRIP,
    kTriangleFan   = GL_TRIANGLE_FAN
};

class JDL_API Mesh : public Resource
{
public:
    /**
     * @brief Creates the mesh.
     * @param name          Mesh name
     * @param primitiveType Mesh primitive type
     */
    Mesh(const std::string& name, PrimitiveType primitiveType = PrimitiveType::kTriangles);

    /**
     * @brief Returns the mesh primitive type.
     */
    PrimitiveType getPrimitiveType() const { return m_primitiveType; }

    /**
     * @brief Adds a new vertex.
     * @param vertex The new vertex
     */
    void addVertex(const core::Vertex& vertex);

    /**
     * @brief Adds multiple vertices.
     * @param start Vertex sequence iterator start
     * @param end   Vertex sequence iterator end
     */
    template<typename Iter>
    void addVertices(Iter start, Iter end)
    {
        m_vertices.insert(m_vertices.cend(), start, end);
        m_reloadVertices = true;
    }

    /**
     * @brief Adds multiple vertices.
     * @param vertices Vertex sequence
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
     * @param start Index sequence iterator start
     * @param end   Index sequence iterator end
     */
    template<typename Iter>
    void addIndices(Iter start, Iter end)
    {
        m_indices.insert(m_indices.cend(), start, end);
        m_reloadIndices = true;
    }

    /**
     * @brief Adds multiple indices.
     * @param indices Index sequence
     */
    void addIndices(const std::vector<uint32_t>& indices) {
        addIndices(indices.cbegin(), indices.cend());
    }

    /**
     * @brief Returns the number of vertices.
     */
    size_t getNbVertices() const { return m_vertices.size(); }

    /**
     * @brief Returns the number of indices.
     */
    size_t getNbIndices() const { return m_indices.size(); }

    /**
     * @brief Returns the number of primitives.
     */
    size_t getNbPrimitives() const;

    /**
     * @brief Renders the mesh.
     */
    void render();

    /**
     * @brief Clears the mesh data.
     */
    void clear();

private:
    // Mesh info
    PrimitiveType m_primitiveType;

    // Mesh data
    std::vector<core::Vertex> m_vertices;
    std::vector<uint32_t> m_indices;

    // Vertex/Index buffers dirtiness flags
    bool m_reloadVertices;
    bool m_reloadIndices;

    // OpenGL data
    GLuint m_vertexArray;
    GLuint m_vertexBuffer;
    GLuint m_indexBuffer;

    // Setups the mesh (creates the OpenGL objects, setups the vertex buffer description)
    void setup();
    // Updates the vertex/index buffers if necessary
    void updateBuffers();
    // Clears the mesh OpenGL data
    void clearResource();
};

} // namespace resource
} // namespace jdl
