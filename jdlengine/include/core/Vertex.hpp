#pragma once

#include <glm/glm.hpp>


namespace jdl
{
namespace core
{

struct Vertex
{
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 normal = glm::vec3(0.0f);
    glm::vec2 uv = glm::vec2(0.0f);

    Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& uv)
        : position(position)
        , normal(normal)
        , uv(uv)
    {}
};

} // namespace core
} // namespace jdl
