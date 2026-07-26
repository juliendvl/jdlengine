#pragma once

#include "resource.hpp"

#include <any>
#include <typeindex>
#include <unordered_map>


namespace jdl
{
namespace resource
{

class ResourceManager
{
public:
	/**
	 * @brief Creates a resource of type R and stores it in the manager.
	 * @param name	Desired resource name. If the name is already in use, a numeric
	 *				suffix is appended until a unique name is found.
	 * @param args	Arguments forwarded to the resource constructor.
	 * @return Pointer to the created resource.
	 */
	template<class R, typename... Args>
	static R* Create(const std::string& name, Args&&... args)
	{
		auto& bucket = Get().get_bucket<R>();

		// Find a valid resource name
		std::string valid_name = name;
		size_t index = 1;

		while (bucket.get(valid_name) != nullptr) {
			valid_name = name + std::to_string(index++);
		}

		auto resource = std::make_unique<R>(valid_name, std::forward<Args>(args)...);
		bucket.resources[valid_name] = std::move(resource);

		return static_cast<R*>(bucket.get(valid_name));
	}

	/**
	 * @brief Returns a resource of type R by name.
	 * @param	name Resource name.
	 * @return	Pointer to the resource, or nullptr if no resource with that name
	 *			exists.
	 */
	template<class R>
	static R* Get(const std::string& name)
	{
		auto& bucket = Get().get_bucket<R>();
		return bucket.get(name);
	}

	/**
	 * @brief Returns all resources of type R currently stored in the manager.
	 * @return Vector of pointers to every resource of type R.
	 */
	template<class R>
	static std::vector<R*> GetAll()
	{
		std::vector<R*> resources;
		for (const auto& [name, resource] : Get().get_bucket<R>().resources) {
			resources.push_back(static_cast<R*>(resource.get()));
		}
		return resources;
	}

	/**
	 * @brief Removes a resource of type R by name.
	 * @param name Resource name.
	 */
	template<class R>
	static void Remove(const std::string& name)
	{
		auto& bucket = Get().get_bucket<R>();
		bucket.remove(name);
	}

	/**
	 * @brief Removes and clears all resources managed by every type bucket.
	 */
	static void Clear()
	{
		for (auto& [type, bucket] : Get().m_buckets) {
			bucket.clear();
		}
		Get().m_buckets.clear();
	}

private:
	static ResourceManager& Get()
	{
		static ResourceManager s_Manager;
		return s_Manager;
	}

	struct Bucket
	{
		std::unordered_map<std::string, std::unique_ptr<Resource>> resources;

		Resource* get(const std::string& name) const
		{
			auto it = resources.find(name);
			return it != resources.end() ? it->second.get() : nullptr;
		}

		void remove(const std::string& name)
		{
			auto it = resources.find(name);
			if (it != resources.end())
			{
				it->second->clear_resource();
				resources.erase(it);
			}
		}

		void clear()
		{
			for (const auto& [name, resource] : resources) {
				resource->clear_resource();
			}
			resources.clear();
		}
	};
	std::unordered_map<std::type_index, Bucket> m_buckets;

	template<class R>
	Bucket& get_bucket()
	{
		auto resource_type = std::type_index(typeid(R));
		return m_buckets[resource_type];
	}
};

} // namespace resource
} // namespace jdl
