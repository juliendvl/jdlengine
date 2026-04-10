#pragma once

#include "utils/NonCopyable.hpp"


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
	 * @brief Renders a frame.
	 */
	void renderFrame();
};

} // namespace vk
} // namespace jdl
