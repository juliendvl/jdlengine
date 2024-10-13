#include "resource/Mesh.hpp"

#include "utils/Logger.hpp"


namespace jdl
{
namespace resource
{

Mesh::Mesh(const std::string& name, PrimitiveType primitiveType)
    : Resource(name)
    , m_primitiveType(primitiveType)
    , m_verticesDirty(false)
    , m_indicesDirty(false)
    , m_vao(0)
    , m_vbo(0)
    , m_ibo(0)
    , m_material(nullptr)
{
    setupBuffers();
}

void Mesh::addVertex(const core::Vertex& vertex)
{
    m_vertices.push_back(vertex);
    m_verticesDirty = true;
}

void Mesh::addIndex(uint32_t index)
{
    m_indices.push_back(index);
    m_indicesDirty = true;
}

size_t Mesh::getNbPrimitives() const
{
    size_t nbIndices = m_indices.size();

    switch (m_primitiveType)
    {
        case PrimitiveType::kPoints:
            return nbIndices;
        case PrimitiveType::kLines:
            return nbIndices / 2;
        case PrimitiveType::kTriangles:
            return nbIndices / 3;
        case PrimitiveType::kLineStrip:
            return nbIndices >= 2 ? nbIndices - 1 : 0;
        case PrimitiveType::kLineLoop:
            return nbIndices >= 2 ? nbIndices : 0;
        case PrimitiveType::kTriangleStrip:
        case PrimitiveType::kTriangleFan:
            return nbIndices >= 3 ? nbIndices - 2 : 0;
        default:
            // Should not be raised!
            return 0;
    }
}

void Mesh::render(const core::RenderContext& context)
{
    if (m_material)
    {
        m_material->bind(context);
    }

    glBindVertexArray(m_vao);
    {
        updateBuffers();

        if (m_indices.empty())
        {
            glDrawArrays(static_cast<GLenum>(m_primitiveType), 0, static_cast<GLsizei>(m_vertices.size()));
        }
        else
        {
            glDrawElements(
                static_cast<GLenum>(m_primitiveType),
                static_cast<GLsizei>(m_indices.size()),
                GL_UNSIGNED_INT,
                nullptr
            );
        }
    }
    glBindVertexArray(0);
}

void Mesh::clear()
{
    m_vertices.clear();
    m_indices.clear();

    m_verticesDirty = true;
    m_indicesDirty = true;
}

void Mesh::setupBuffers()
{
    glGenVertexArrays(1, &m_vao);
    if (m_vao == 0)
    {
        JDL_FATAL("Failed to create the OpenGL VAO!");
    }

    glGenBuffers(1, &m_vbo);
    if (m_vbo == 0)
    {
        JDL_FATAL("Failed to create the OpenGL VBO!");
    }

    glGenBuffers(1, &m_ibo);
    if (m_ibo == 0)
    {
        JDL_FATAL("Failed to create the OpenGL IBO!");
    }

    glBindVertexArray(m_vao);
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        {
            GLsizei stride = static_cast<GLsizei>(sizeof(core::Vertex));

            // Position
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
            glEnableVertexAttribArray(0);

            // Normal
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(core::Vertex, normal));
            glEnableVertexAttribArray(1);

            // UV
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(core::Vertex, uv));
            glEnableVertexAttribArray(2);
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    glBindVertexArray(0);
}

void Mesh::updateBuffers()
{
    if (m_verticesDirty)
    {
        m_verticesDirty = false;

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        {
            glBufferData(
                GL_ARRAY_BUFFER,
                m_vertices.size() * sizeof(core::Vertex),
                m_vertices.data(),
                GL_STATIC_DRAW
            );
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    if (m_indicesDirty)
    {
        m_indicesDirty = false;

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            m_indices.size() * sizeof(uint32_t),
            m_indices.data(),
            GL_STATIC_DRAW
        );

        // Only unbind the IBO if there are no indices to draw
        if (m_indices.empty())
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }
    }
}

void Mesh::clearResource()
{
    clear();

    glDeleteBuffers(1, &m_ibo);
    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);
}

} // namespace resource
} // namespace jdl
