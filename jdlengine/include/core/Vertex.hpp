#pragma once

#include <glm/glm.hpp>


namespace jdl
{
namespace core
{

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;

    Vertex() : position(0.0f), normal(0.0f), uv(0.0f) {}

    Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& uv)
        : position(position)
        , normal(normal)
        , uv(uv)
    {}
};

} // namespace core
} // namespace jdl