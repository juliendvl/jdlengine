#include "resource/ResourceManager.hpp"
#include "resource/Resource.hpp"


namespace jdl
{
namespace resource
{

std::unordered_map<std::string, std::unique_ptr<Resource>> ResourceManager::RESOURCES;

Resource* ResourceManager::Get(const std::string& name)
{
	auto it = RESOURCES.find(name);
	return it != RESOURCES.end() ? it->second.get() : nullptr;
}

void ResourceManager::Remove(const std::string& name)
{
	auto it = RESOURCES.find(name);
	if (it != RESOURCES.end())
	{
		it->second->clearResource();
		it->second.reset();
		RESOURCES.erase(it);
	}
}

void ResourceManager::Clear()
{
	for (auto& [name, resource] : RESOURCES)
	{
		resource->clearResource();
		resource.reset();
	}
	RESOURCES.clear();
}

} // namespace resource
} // namespace jdl
