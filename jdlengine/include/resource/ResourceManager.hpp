#pragma once

#include "core/Core.hpp"

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
     * @brief  Creates a new resource, and adds it to the ResourceManager.
     * @param  name Resource name
     * @param  args Resource initialization arguments
     * @return The created resource.
     */
    template<class R, typename... Args>
    static R* Create(const std::string& name, Args&&... args)
    {
        // Find a valid name for the new resource
        std::string validName = name;
        size_t      index = 1;
        while (Get(validName) != nullptr)
        {
            validName = name + std::to_string(index++);
        }

        RESOURCES[validName] = std::make_unique<R>(name, std::forward<Args>(args)...);
        return static_cast<R*>(RESOURCES[validName].get());
    }

    /**
     * @brief  Retrieves and returns a managed resource.
     * @param  name Resource name
     * @return Resource, or nullptr if it does not exist.
     */
    static Resource* Get(const std::string& name);

    /**
     * @brief  Retrieves and returns a managed resource, converted to the desired type.
     * @param  name Resource name
     * @return Resource, or nullptr if it does not exist or if it cannot be converted
     */
    template<class R>
    static R* GetAs(const std::string& name) { return dynamic_cast<R*>(Get(name)); }

    /**
     * @brief Returns all the resources of the desired type.
     */
    template<class R>
    static std::vector<R*> AllOf()
    {
        std::vector<R*> result;
        for (const auto& it : RESOURCES)
        {
            if (auto resource = dynamic_cast<R*>(it.second.get()))
            {
                result.push_back(resource);
            }
        }
        return result;
    }

    /**
     * @brief Removes a resource from the ResourceManager. The resource will be deleted.
     * @param name Resource name
     */
    static void Remove(const std::string& name);

    /**
     * @brief Clears the ResourceManager.
     */
    static void Clear();

private:
    static std::unordered_map<std::string, std::unique_ptr<Resource>> RESOURCES;
};

} // namespace resource
} // namespace jdl
