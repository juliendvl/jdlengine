#pragma once

#include <unordered_map>


namespace jdl
{
namespace resource
{

class Resource;

class ResourceManager
{
public:
	/**
	 * @brief Creates a resource.
	 * @param name The resource name.
	 * @param args Resource initialization arguments.
	 * @return The created resource.
	 */
	template<class R, typename... Args>
	static R* Create(const std::string& name, Args&&... args)
	{
		std::string validName = name;
		size_t index = 1;

		while (Get(validName)) {
			validName = name + std::to_string(index++);
		}

		RESOURCES[validName] = std::make_unique<R>(
			validName,
			std::forward<Args>(args)...
		);
		return static_cast<R*>(RESOURCES[validName].get());
	}

	/**
	 * @brief Finds and returns a resource.
	 * @param name The resource name.
	 * @return The queried resource, or nullptr if no such resource exists.
	 */
	static Resource* Get(const std::string& name);

	/**
	 * @brief Finds and returns a resource, converted to the desired type.
	 * @param name The resource name.
	 * 
	 * @return The queried resource, or nullptr if no such resource exists or
	 * if the resource cannot be converted.
	 */
	template<class R>
	static R* GetAs(const std::string& name) {
		return dynamic_cast<R*>(Get(name));
	}

	/**
	 * @brief Returns all the resources of the desired type.
	 */
	template<class R>
	static std::vector<R*> AllOf()
	{
		std::vector<R*> result;
		for (const auto& [name, resource] : RESOURCES)
		{
			if (auto r = dynamic_cast<R*>(resource)) {
				result.push_back(r);
			}
		}
		return result;
	}

	/**
	 * @brief Deletes a resource.
	 * @param name The resource name.
	 */
	static void Remove(const std::string& name);

	/**
	 * @brief Clears the resource manager.
	 */
	static void Clear();

private:
	static std::unordered_map<std::string, std::unique_ptr<Resource>> RESOURCES;
};

} // namespace resource
} // namespace jdl
