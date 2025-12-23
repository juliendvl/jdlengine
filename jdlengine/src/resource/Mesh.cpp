#include "resource/Mesh.hpp"


namespace jdl
{
namespace resource
{

Mesh::Mesh(const std::string& name) : Resource(name) {}

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

void Mesh::render(core::RenderContext& context)
{
    if (m_vertices.empty()) {
        return;
    }
    updateBuffers();

    m_vertexBuffer->bind(context.commandBuffer);
    if (m_indices.empty()) {
        vkCmdDraw(context.commandBuffer, static_cast<uint32_t>(m_vertices.size()), 1, 0, 0);
    }
    else
    {
        m_indexBuffer->bind(context.commandBuffer);
        vkCmdDrawIndexed(context.commandBuffer, static_cast<uint32_t>(m_indices.size()), 1, 0, 0, 0);
    }
}

void Mesh::updateBuffers()
{
    if (m_reloadVertices)
    {
        m_vertexBuffer.reset();
        m_vertexBuffer = std::make_unique<vk::VulkanVertexBuffer>(getNbVertices() * sizeof(core::Vertex));
        m_vertexBuffer->setData(m_vertices.data());

        m_reloadVertices = false;
    }
    if (m_reloadIndices)
    {
        m_indexBuffer.reset();
        m_indexBuffer = std::make_unique<vk::VulkanIndexBuffer>(getNbIndices() * sizeof(uint32_t));
        m_indexBuffer->setData(m_indices.data());

        m_reloadIndices = false;
    }
}

void Mesh::clearResource()
{
    if (m_vertexBuffer) {
        m_vertexBuffer.reset();
    }
    if (m_indexBuffer) {
        m_indexBuffer.reset();
    }
}

} // namespace resource
} // namespace jdl
