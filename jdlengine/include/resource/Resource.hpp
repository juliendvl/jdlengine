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
    /**
     * @brief Safely destroys the resource.
     */
    ~Resource() { clearResource(); }

protected:
    /**
     * @brief Base constructor.
     * @param name Resource name
     */
    Resource(const std::string& name) : core::Object(name) {}

    /**
     * @brief Clears the resource data. Must be reimplemented if necessary.
     */
    virtual void clearResource() {}
};

} // namespace resource
} // namespace jdl
