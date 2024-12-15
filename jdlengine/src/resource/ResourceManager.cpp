#include "resource/ResourceManager.hpp"
#include "resource/Resource.hpp"


namespace jdl
{
namespace resource
{

std::unordered_map<std::string, std::unique_ptr<Resource>> ResourceManager::IResources;

Resource* ResourceManager::Get(const std::string& name)
{
    auto it = IResources.find(name);
    return it != IResources.end() ? it->second.get() : nullptr;
}

void ResourceManager::Remove(const std::string& name)
{
    auto it = IResources.find(name);
    if (it != IResources.end())
    {
        IResources.erase(it);
    }
}

void ResourceManager::Clear()
{
    for (const auto& [name, resource] : IResources)
    {
        resource->clearResource();
    }
    IResources.clear();
}

} // namespace resource
} // namespace jdl
