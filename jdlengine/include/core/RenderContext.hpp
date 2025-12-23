#pragma once

#include "Core.hpp"


namespace jdl
{
namespace core
{

struct RenderContext
{
    // Current command buffer
    VkCommandBuffer commandBuffer = VK_NULL_HANDLE;
};

} // namespace core
} // namespace jdl
