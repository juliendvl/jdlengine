#pragma once

#include "core/Object.hpp"


namespace jdl
{
namespace resource
{

class JDL_API Resource : public core::Object
{
    friend class ResourceManager;

public:
    // Base destructor
    ~Resource() { clearResource(); }

protected:
    // Base constructor
    Resource(const std::string& name) : core::Object(name) {}

    // Clears the resource data. Must be reimplemented if necessary
    virtual void clearResource() {}
};

} // namespace resource
} // namespace jdl
