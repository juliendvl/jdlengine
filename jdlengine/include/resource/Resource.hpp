#pragma once

#include "core/Object.hpp"

#include "ResourceManager.hpp"


namespace jdl
{
namespace resource
{

class JDL_API Resource : public core::Object
{
	friend class ResourceManager;

public:
	~Resource() { clearResource(); }

protected:
	Resource(const std::string& name) : core::Object(name) {}

	/**
	 * @brief Clears the resource data.
	 */
	virtual void clearResource() {}
};

} // namespace resource
} // namespace jdl
