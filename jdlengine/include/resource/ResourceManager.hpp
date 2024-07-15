#pragma once

#include "core/Object.hpp"

#include <unordered_map>

#include "utils/NonCopyable.hpp"


namespace jdl
{
namespace resource
{

class Resource;

class JDL_API ResourceManager : private NonCopyable<ResourceManager>
{
public:
    /**
     * @brief Creates a new resource and adds it to the resource manager.
     * @param name Resource name.
     * @param args Resource initialization arguments.
     */
    template<class R, typename... Args>
    static R* Create(const std::string& name, Args&&... args)
    {
        // Find a valid resource name
        std::string validName = name;
        size_t index = 1;

        while (Get(validName) != nullptr)
        {
            validName = name + "_" + std::to_string(index++);
        }

        R* resource = new R(validName, std::forward<Args>(args)...);
        sResources[validName] = std::unique_ptr<Resource>(resource);

        return resource;
    }

    /**
     * @brief Returns a resource from its name.
     * @param name Resource name.
     * @return Resource, or nullptr if it does not exist.
     */
    static Resource* Get(const std::string& name);

    /**
     * @brief Returns a resource from its name, converted to the required resource type.
     * @param name Resource name.
     * @return Resource, or nullptr if it does not exist or if it cannot be converted.
     */
    template<class R>
    static R* As(const std::string& name)
    {
        return dynamic_cast<R*>(Get(name));
    }

    /**
     * @brief Returns all the resources of the desired type.
     */
    template<class R>
    static std::vector<R*> AllOf()
    {
        std::vector<R*> result;
        for (const auto& [name, resource] : sResources)
        {
            R* converted = dynamic_cast<R*>(resource.get());
            if (converted)
            {
                result.push_back(converted);
            }
        }
        return result;
    }

    /**
     * @brief Deletes a resource.
     * @param name Resource name.
     */
    static void Remove(const std::string& name);

    /**
     * @brief Removes all the resources.
     */
    static void Clear();

private:
    static std::unordered_map<std::string, std::unique_ptr<Resource>> sResources;
};

} // namespace resource
} // namespace jdl
