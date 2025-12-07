#pragma once

#include "Core.hpp"

#include "utils/NonCopyable.hpp"


namespace jdl
{
namespace core
{

class JDL_API Object : private NonCopyable<Object>, private std::enable_shared_from_this<Object>
{
public:
	/**
	 * @brief Returns the object name.
	 */
	const std::string& getName() const {
		return m_name;
	}

	/**
	 * @brief Returns a valid shared pointer from the instance.
	 */
	std::shared_ptr<Object> getPointer() { return shared_from_this(); }
	std::shared_ptr<const Object> getPointer() const { return shared_from_this(); }

protected:
	Object(const std::string& name) : m_name(name) {}

private:
	std::string m_name;
};

} // namespace core
} // namespace jdl
