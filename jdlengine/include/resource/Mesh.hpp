#pragma once

#include "Resource.hpp"

#include "core/RenderContext.hpp"
#include "core/Vertex.hpp"

#include "Material.hpp"


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
     * @brief Returns the primitive type.
     */
    PrimitiveType getPrimitiveType() const {
        return m_primitiveType;
    }

    /**
     * @brief Adds a new vertex.
     */
    void addVertex(const core::Vertex& vertex);

    /**
     * @brief Adds multiple vertices.
     * @param start Vertices sequence start iterator
     * @param end   Vertices sequence end iterator
     */
    template<typename Iter>
    void addVertices(Iter start, Iter end)
    {
        m_vertices.insert(m_vertices.cbegin(), start, end);
        m_verticesDirty = true;
    }

    /**
     * @brief Adds multiple vertices.
     * @param vertices Vertices array
     */
    void addVertices(const std::vector<core::Vertex>& vertices) {
        addVertices(vertices.cbegin(), vertices.cend());
    }

    /**
     * @brief Adds a new vertex index.
     */
    void addIndex(uint32_t index);

    /**
     * @brief Adds multiple indices.
     * @param start Indices sequence start iterator
     * @param end   Indices sequence end iterator
     */
    template<typename Iter>
    void addIndices(Iter start, Iter end)
    {
        m_indices.insert(m_indices.cbegin(), start, end);
        m_indicesDirty = true;
    }

    /**
     * @brief Adds multiple indices.
     * @param indices Indice sarray
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
     * @brief Returns the number of primitives.
     */
    size_t getNbPrimitives() const;

    /**
     * @brief Returns the mesh material.
     */
    Material* getMaterial() const {
        return m_material;
    }

    /**
     * @brief Sets the mesh material.
     */
    void setMaterial(Material* material) {
        m_material = material;
    }

    /**
     * @brief Renders the mesh content.
     * @param context Render context
     */
    void render(const core::RenderContext& context);

    /**
     * @brief Clears the mesh data (vertices and indices).
     */
    void clear();

private:
    PrimitiveType m_primitiveType;
    
    std::vector<core::Vertex> m_vertices;
    std::vector<uint32_t>     m_indices;

    bool m_verticesDirty;
    bool m_indicesDirty;

    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_ibo;

    Material* m_material;

    // Creates and setups the OpenGL buffers
    void setupBuffers();

    // Clears the resource data
    void clearResource();
    // Updates the buffers content
    void updateBuffers();
};

} // namespace resource
} // namespace jdl
