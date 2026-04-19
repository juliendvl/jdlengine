#pragma once

#include "core/Object.hpp"


namespace jdl
{
namespace resource
{

class Resource : public core::Object
{
	friend class ResourceManager;

public:
	Resource(const std::string& name) : core::Object(name) {}

	~Resource() { clearResource(); }

protected:
	/**
	 * @brief Clears the resource data. Must be reimplemented if necessary.
	 */
	virtual void clearResource() {}
};

} // namespace resource
} // namespace jdl
