#pragma once

#include "Core.hpp"


namespace jdl
{
namespace core
{

struct Vertex
{
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 normal = glm::vec3(0.0f);
    glm::vec2 uv = glm::vec2(0.0f);

    Vertex() = default;
    
    Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& uv)
        : position(position)
        , normal(normal)
        , uv(uv)
    {}

    /**
     * @brief Returns the vertex binding descriptions.
     */
    static std::vector<VkVertexInputBindingDescription> GetBindingDescriptions()
    {
        VkVertexInputBindingDescription binding = {};
        binding.binding = 0;
        binding.stride = sizeof(Vertex);
        binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return {binding};
    }

    /**
     * @brief Returns the vertex attribute descriptions.
     */
    static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions()
    {
        // Position
        VkVertexInputAttributeDescription positionAttribute = {};
        positionAttribute.binding = 0;
        positionAttribute.location = 0;
        positionAttribute.format = VK_FORMAT_R32G32B32_SFLOAT;
        positionAttribute.offset = offsetof(Vertex, position);

        // Normal
        VkVertexInputAttributeDescription normalAttribute = {};
        normalAttribute.binding = 0;
        normalAttribute.location = 1;
        normalAttribute.format = VK_FORMAT_R32G32B32_SFLOAT;
        normalAttribute.offset = offsetof(Vertex, normal);

        // UV
        VkVertexInputAttributeDescription uvAttribute = {};
        uvAttribute.binding = 0;
        uvAttribute.location = 2;
        uvAttribute.format = VK_FORMAT_R32G32_SFLOAT;
        uvAttribute.offset = offsetof(Vertex, uv);

        return {positionAttribute, normalAttribute, uvAttribute};
    }
};

} // namespace core
} // namespace jdl
