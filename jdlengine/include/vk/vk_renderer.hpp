#pragma once

#include <memory>

#include "utils/non_copyable.hpp"

#include "vk_common.hpp"
#include "vk_context.hpp"


namespace jdl
{
namespace vk
{

class Renderer : private NonCopyable<Renderer>
{
public:
	/**
	 * @brief Creates the renderer and its Vulkan context.
	 * @param window The window to render to. It must outlive the renderer.
	 */
	explicit Renderer(const core::Window& window);

	/**
	 * @brief Destroys the renderer.
	 */
	~Renderer();

	/**
	 * @brief Returns the Vulkan context.
	 */
	Context& context() const { return *m_context; }

	/**
	 * @brief Draws a single frame.
	 */
	void draw_frame();

private:
	std::unique_ptr<Context> m_context;
};

} // namespace vk
} // namespace jdl
