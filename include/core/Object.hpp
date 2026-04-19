#pragma once

#include "utils/NonCopyable.hpp"


namespace jdl
{
namespace core
{

class Object : private NonCopyable<Object>
{
public:
	/**
	 * @brief Returns the object name.
	 */
	const std::string& getName() const { return m_name; }

protected:
	Object(const std::string& name) : m_name(name) {}

private:
	std::string m_name;
};

} // namespace core
} // namespace jdl
