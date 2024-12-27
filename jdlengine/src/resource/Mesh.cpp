#include "resource/Mesh.hpp"

#include "utils/Logger.hpp"


namespace jdl
{
namespace resource
{

Mesh::Mesh(const std::string& name, PrimitiveType primitiveType)
    : Resource(name)
    , m_primitiveType(primitiveType)
    , m_reloadVertices(false)
    , m_reloadIndices(false)
    , m_vertexArray(0)
    , m_vertexBuffer(0)
    , m_indexBuffer(0)
{
    setup();
}

void Mesh::addVertex(const core::Vertex& vertex)
{
    m_vertices.push_back(vertex);
    m_reloadVertices = true;
}

void Mesh::addIndex(uint32_t index)
{
    m_indices.push_back(index);
    m_reloadIndices = true;
}

size_t Mesh::getNbPrimitives() const
{
    size_t nbIndices = getNbIndices();

    switch (m_primitiveType)
    {
        case PrimitiveType::kPoints:
            return nbIndices;
        case PrimitiveType::kLines:
            return nbIndices / 2;
        case PrimitiveType::kLineStrip:
            return nbIndices >= 2 ? nbIndices - 1 : 0;
        case PrimitiveType::kLineLoop:
            return nbIndices >= 2 ? nbIndices : 0;
        case PrimitiveType::kTriangles:
            return nbIndices / 3;
        case PrimitiveType::kTriangleStrip:
        case PrimitiveType::kTriangleFan:
            return nbIndices >= 3 ? nbIndices - 2 : 0;
        default:
            JDL_FATAL("Should not be raised!");
            return 0;
    }
}

void Mesh::render()
{
    glBindVertexArray(m_vertexArray);
    {
        updateBuffers();

        if (m_indices.empty())
        {
            GLsizei size = static_cast<GLsizei>(m_vertices.size());
            glDrawArrays(static_cast<GLenum>(m_primitiveType), 0, size);
        }
        else
        {
            GLsizei size = static_cast<GLsizei>(m_indices.size());

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
            glDrawElements(static_cast<GLenum>(m_primitiveType), size, GL_UNSIGNED_INT, nullptr);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }
    }
    glBindVertexArray(0);
}

void Mesh::clear()
{
    m_vertices.clear();
    m_indices.clear();

    m_reloadVertices = true;
    m_reloadIndices = true;
}

void Mesh::setup()
{
    glCreateVertexArrays(1, &m_vertexArray);
    glCreateBuffers(1, &m_vertexBuffer);
    glCreateBuffers(1, &m_indexBuffer);

    if (m_vertexArray == 0)
    {
        JDL_FATAL("Failed to create the OpenGL vertex array");
    }
    if (m_vertexBuffer == 0)
    {
        JDL_FATAL("Failed to create the OpenGL VBO");
    }
    if (m_indexBuffer == 0)
    {
        JDL_FATAL("Failed to create the OpenGL IBO");
    }

    glBindVertexArray(m_vertexArray);
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);

        // Position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(core::Vertex), (void*)offsetof(core::Vertex, position));
        // Normal
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(core::Vertex), (void*)offsetof(core::Vertex, normal));
        // UV
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(core::Vertex), (void*)offsetof(core::Vertex, uv));
    }
    glBindVertexArray(0);
}

void Mesh::updateBuffers()
{
    if (m_reloadVertices)
    {
        GLsizei size = static_cast<GLsizei>(m_vertices.size() * sizeof(core::Vertex));
        glBufferData(GL_ARRAY_BUFFER, size, m_vertices.data(), GL_STATIC_DRAW);
    }
    if (m_reloadIndices)
    {
        GLsizei size = static_cast<GLsizei>(m_indices.size() * sizeof(uint32_t));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, m_indices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}

void Mesh::clearResource()
{
    glDeleteBuffers(1, &m_indexBuffer);
    glDeleteBuffers(1, &m_vertexBuffer);
    glDeleteVertexArrays(1, &m_vertexArray);
}

} // namespace resource
} // namespace jdl
