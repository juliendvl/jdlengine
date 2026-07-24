#pragma once

#include "utils/non_copyable.hpp"


namespace jdl
{
namespace core
{

class Object : private NonCopyable<Object>, std::enable_shared_from_this<Object>
{
public:
	/**
	 * @brief Returns the object name.
	 */
	const std::string& get_name() const { return m_name; }

	/**
	 * @brief Returns a valid shared pointer from the current instance.
	 */
	std::shared_ptr<Object> get_pointer() { return shared_from_this(); }
	std::shared_ptr<const Object> get_pointer() const { return shared_from_this(); }

protected:
	// Base constructor
	Object(const std::string& name) : m_name(name) {}

private:
	std::string m_name;
};

} // namespace core
} // namespace jdl
