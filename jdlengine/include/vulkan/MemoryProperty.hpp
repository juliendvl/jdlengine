#pragma once

#include "core/Core.hpp"


namespace jdl
{
namespace vk
{

enum MemoryProperty
{
    eDeviceLocal = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
    eHostCoherent = VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
    eHostVisible = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
};

inline MemoryProperty operator&(MemoryProperty a, MemoryProperty b) { return (MemoryProperty)((int)a & (int)b); }
inline MemoryProperty operator|(MemoryProperty a, MemoryProperty b) { return (MemoryProperty)((int)a | (int)b); }

inline MemoryProperty& operator&=(MemoryProperty& a, MemoryProperty b)
{
    a = a & b;
    return a;
}

inline MemoryProperty& operator|=(MemoryProperty& a, MemoryProperty b)
{
    a = a | b;
    return a;
}

} // namespace vk
} // namespace jdl
