#pragma once

#include "core/Core.hpp"

#include "utils/NonCopyable.hpp"

#include <unordered_map>


namespace jdl
{
namespace resource
{

class Resource;

class JDL_API ResourceManager : private NonCopyable<ResourceManager>
{
public:
	/**
	 * @brief Creates and returns a new resource.
	 * @param name The resource desired name.
	 * @param args Initialization arguments.
	 * @return The created resource.
	 */
	template<class R, typename... Args>
	static R* Create(const std::string& name, Args&&... args)
	{
		// Find a valid name for the resource.
		std::string validName = name;
		size_t index = 1;
		while (Get(validName) != nullptr) {
			validName = name + std::to_string(index++);
		}

		RESOURCES[validName] = std::make_unique<R>(name, std::forward<Args>(args)...);
		return static_cast<R*>(RESOURCES[validName].get());
	}

	/**
	 * @brief Finds and returns a resource.
	 * @param name The resource name.
	 * @return The desired resource, or nullptr if no such resource exists.
	 */
	static Resource* Get(const std::string& name);

	/**
	 * @brief Finds and returns a resource, converted to the desired type.
	 * @param name The resource name.
	 * @return The desired resource, or nullptr if no such resource exists or if it cannot be converted.
	 */
	template<class R>
	static R* GetAs(const std::string& name) {
		return dynamic_cast<R*>(Get(name));
	}

	/**
	 * @brief Returns all the resources of the desired type.
	 */
	template<class R>
	static std::vector<R*> GetAll()
	{
		std::vector<R*> result;
		for (const auto& [name, resource] : RESOURCES) {
			if (auto r = dynamic_cast<R*>(resource.get())) {
				result.push_back(r);
			}
		}
		return result;
	}

	/**
	 * @brief Clears and removes a resource.
	 * @param name The resource name.
	 */
	static void Remove(const std::string& name);

	/**
	 * @brief Clears all the resources.
	 */
	static void Clear();

private:
	static std::unordered_map<std::string, std::unique_ptr<Resource>> RESOURCES;
};

} // namespace resource
} // namespace jdl
