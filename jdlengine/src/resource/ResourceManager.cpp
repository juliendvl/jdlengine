#include "resource/ResourceManager.hpp"
#include "resource/Resource.hpp"

#include "utils/Logger.hpp"


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
		RESOURCES.erase(it);
	}
}

void ResourceManager::Clear()
{
	for (const auto& [name, resource] : RESOURCES) {
		resource->clearResource();
	}
	RESOURCES.clear();
}

} // namespace resource
} // namespace jdl
