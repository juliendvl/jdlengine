#include "resource/ResourceManager.hpp"
#include "resource/Resource.hpp"

#include "utils/Logger.hpp"


namespace jdl
{
namespace resource
{

std::unordered_map<std::string, std::unique_ptr<Resource>> ResourceManager::sResources;

Resource* ResourceManager::Get(const std::string& name)
{
    auto it = sResources.find(name);
    return it != sResources.end() ? it->second.get() : nullptr;
}

void ResourceManager::Remove(const std::string& name)
{
    auto it = sResources.find(name);
    if (it != sResources.end())
    {
        it->second->clearResource();
        sResources.erase(it);
    }
}

void ResourceManager::Clear()
{
    for (const auto& [name, resource] : sResources)
    {
        resource->clearResource();
    }
    sResources.clear();
}

} // namespace resource
} // namespace jdl
