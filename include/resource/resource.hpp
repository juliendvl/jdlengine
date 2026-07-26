#pragma once

#include "core/object.hpp"


namespace jdl
{
namespace resource
{

class Resource : public core::Object
{
	friend class ResourceManager;

public:
	// Base destructor
	~Resource() { clear_resource(); }

protected:
	// Base constructor
	Resource(const std::string& name) : core::Object(name) {}

	/**
	 * @brief Clears the resource data. Must be reimplemented if necessary.
	 */
	virtual void clear_resource() {}
};

} // namespace resource
} // namespace jdl
