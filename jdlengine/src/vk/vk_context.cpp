#include "vk/vk_context.hpp"


namespace jdl
{
namespace vk
{

Context::Context()
{
	m_instance = std::make_unique<Instance>();
}

Context::~Context()
{
	m_instance.reset();
}

} // namespace vk
} // namespace jdl
