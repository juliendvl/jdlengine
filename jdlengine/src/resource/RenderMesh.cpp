#include "resource/RenderMesh.hpp"


namespace jdl
{
namespace resource
{

RenderMesh::RenderMesh(const std::string& name)
    : Resource(name)
{}

void RenderMesh::addVertex(const core::Vertex& vertex)
{
    m_vertices.push_back(vertex);
    m_verticesDirty = true;
}

void RenderMesh::addIndex(uint32_t index)
{
    m_indices.push_back(index);
    m_indicesDirty = true;
}

void RenderMesh::render(core::RenderContext& context)
{
    updateBuffers();

    m_vertexBuffer->bind(context.commandBuffer);
    if (getNbIndices() == 0)
    {
        vkCmdDraw(context.commandBuffer, static_cast<uint32_t>(getNbVertices()), 1, 0, 0);
    }
    else
    {
        m_indexBuffer->bind(context.commandBuffer);
        vkCmdDrawIndexed(context.commandBuffer, static_cast<uint32_t>(getNbIndices()), 1, 0, 0, 0);
    }
}

void RenderMesh::updateBuffers()
{
    if (m_verticesDirty)
    {
        uint64_t size = static_cast<uint64_t>(getNbVertices() * sizeof(core::Vertex));

        m_vertexBuffer.reset();
        m_vertexBuffer = std::make_unique<core::VertexBuffer>(size);
        m_vertexBuffer->setData((void*)m_vertices.data());

        m_verticesDirty = false;
    }
    if (m_indicesDirty)
    {
        uint64_t size = static_cast<uint64_t>(getNbIndices() * sizeof(uint32_t));

        m_indexBuffer.reset();
        m_indexBuffer = std::make_unique<core::IndexBuffer>(size);
        m_indexBuffer->setData((void*)m_indices.data());

        m_indicesDirty = false;
    }
}

void RenderMesh::clearResource()
{
    m_indexBuffer.reset();
    m_vertexBuffer.reset();
}

} // namespace resource
} // namespace jdl
