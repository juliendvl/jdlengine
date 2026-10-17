#pragma once

#include <memory>

#include "utils/non_copyable.hpp"

#include "vk_common.hpp"
#include "vk_instance.hpp"


namespace jdl
{
namespace vk
{

class Context : private NonCopyable<Context>
{
public:
	/**
	 * @brief Creates the Vulkan context and its underlying objects.
	 */
	Context();

	/**
	 * @brief Destroys the Vulkan context.
	 */
	~Context();

	/**
	 * @brief Returns the Vulkan instance.
	 */
	Instance& instance() const { return *m_instance; }

private:
	std::unique_ptr<Instance> m_instance;
};

} // namespace vk
} // namespace jdl
