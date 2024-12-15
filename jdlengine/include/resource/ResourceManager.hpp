#pragma once

#include "core/Core.hpp"
#include "utils/NonCopyable.hpp"

#include <unordered_map>


namespace jdl
{
namespace resource
{

class Resource;

class JDL_API ResourceManager : private utils::NonCopyable<ResourceManager>
{
public:
    /**
     * @brief Creates and returns a new resource.
     * @param  name Resource name
     * @param  args Resource initialization arguments
     * @return The created resource
     */
    template<class R, typename... Args>
    static R* Create(const std::string& name, Args&&... args)
    {
        // Find a valid name for the new resource
        std::string validName = name;
        size_t index = 1;

        while (Get(validName) != nullptr)
        {
            validName = name + std::to_string(index++);
        }

        auto resource = std::make_unique<R>(validName, std::forward<Args>(args)...);
        IResources[validName] = std::move(resource);

        return static_cast<R*>(IResources.at(validName).get());
    }

    /**
     * @brief  Returns a resource.
     * @param  name Resource name
     * @return Queried resource, or nullptr if it does not exist
     */
    static Resource* Get(const std::string& name);

    /**
     * @brief  Returns a resource, converted to the required type.
     * @param  name Resource name
     * @return Queried resource, or nullptr if it does not exist or if the type is not compatible
     */
    template<class R>
    static R* As(const std::string& name) { return dynamic_cast<R*>(Get(name)); }

    /**
     * @brief Returns all the resources of the required type.
     */
    template<class R>
    static std::vector<R*> AllOf()
    {
        std::vector<R*> result;
        for (const auto& [name, resource] : IResources)
        {
            auto r = dynamic_cast<R*>(resource.get());
            if (r != nullptr)
            {
                result.push_back(r);
            }
        }
        return result;
    }

    /**
     * @brief Removes a resource.
     * @param name Resource name
     */
    static void Remove(const std::string& name);

    /**
     * @brief Clears all the managed resources.
     */
    static void Clear();

private:
    static std::unordered_map<std::string, std::unique_ptr<Resource>> IResources;
};

} // namespace resource
} // namespace jdl
