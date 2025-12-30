#pragma once

#include <glm/glm.hpp>


namespace jdl
{
namespace core
{

struct GlobalUniformBuffer
{
    alignas(16) glm::mat4 viewMatrix;
    alignas(16) glm::mat4 projectionMatrix;
    alignas(16) glm::vec3 cameraPosition;
};

} // namespace core
} // namespace jdl
