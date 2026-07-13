#pragma once

#include "utils/non_copyable.hpp"


namespace jdl
{
namespace vk
{

class VulkanRenderer : private NonCopyable<VulkanRenderer>
{
public:
    VulkanRenderer();
    ~VulkanRenderer();

    /**
     * @brief Renders a new frame.
     */
    void render_frame();
};

} // namespace vk
} // namespace jdl
