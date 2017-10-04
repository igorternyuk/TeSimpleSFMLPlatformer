/* 
 * File:   ResourceManager.hpp
 * Author: igor
 *
 * Created on 1-10-2017., 8:10
 */

#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP
#include <map>
#include <memory>
#include <stdexcept>
#include <cassert>
#include <iostream>

template <typename Identifier, typename Resource>
class ResourceManager
{
    public:
        ResourceManager() = default;
        void load(Identifier id, const std::string &fileName);
        Resource& get(Identifier id);
        const Resource& get(Identifier id) const;
    private:
        std::map<Identifier, std::unique_ptr<Resource>> mResourceMap;
};

template <typename Identifier, typename Resource> 
void ResourceManager<Identifier, Resource>::load(Identifier id,
                                                 const std::string &fileName)
{
    std::unique_ptr<Resource> resource{std::make_unique<Resource>()};
    if(!resource->loadFromFile(fileName))
    {
        throw std::runtime_error("ResourceManager::load - Failed to load: " +
                fileName);       
    }
    auto inserted = mResourceMap.insert(std::make_pair(id, std::move(resource)));
    assert(inserted.second);
}
template <typename Identifier, typename Resource> 
Resource& ResourceManager<Identifier, Resource>::get(Identifier id)
{
    auto found = mResourceMap.find(id);
    assert(found != mResourceMap.end());
    return *found->second;
}

template <typename Identifier, typename Resource> 
const Resource& ResourceManager<Identifier, Resource>::get(Identifier id) const
{
    auto found = mResourceMap.find(id);
    assert(found != mResourceMap.end());
    return *found->second;
}
#endif /* RESOURCEMANAGER_HPP */

