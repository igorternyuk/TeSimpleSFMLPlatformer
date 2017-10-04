/* 
 * File:   composition.hpp
 * Author: igor
 *
 * Created on 2-10-2017., 10:35
 * This file based on video  https://www.youtube.com/watch?v=QAmtgvwHInM
 * from Vittorio Romeo
 */

#ifndef COMPOSITION_HPP
#define COMPOSITION_HPP

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>
#include <cassert>
#include <type_traits>

namespace ecs
{
    struct Component;
    class Entity;
    class EntityManager;
    
    using ComponentID = std::size_t;
    using GroupID = std::size_t;

    namespace Internal
    {
        inline ComponentID getUniqueComponentID() noexcept
        {
            static ComponentID lastID{0u};
            return lastID++;
        }
    }

    template <typename T>
    inline ComponentID getComponentTypeID() noexcept
    {
        static_assert(std::is_base_of<Component, T>::value,
                "T must inherit from Component");
        static ComponentID typeID{Internal::getUniqueComponentID()};
        return typeID;
    }

    constexpr std::size_t maxComponents{32};
    using ComponentBitset = std::bitset<maxComponents>;
    using ComponentArray = std::array<Component*,maxComponents>;
    constexpr std::size_t maxGroups{32};
    using GroupBitset = std::bitset<maxGroups>;

    struct Component
    {
        Entity* mEntity;
        virtual ~Component() {}
        virtual void init() {}
        virtual void update(float frameTime) {}
        virtual void draw() {}
    };

    class Entity
    {
    public:
        explicit Entity(EntityManager &manager);
        void update(float frameTime);
        void draw();
        inline bool isAlive() const { return mIsAlive; }
        inline void destroy() { mIsAlive = false; }
        template<typename T>
        bool hasComponent() const
        {
            return mComponentBitset[getComponentTypeID<T>()];
        }

        bool hasGroup(GroupID id)
        {
            return mGroupBitset[id];
        }
        
        void addGroup(GroupID id) noexcept;
        
        inline void deleteGroup(GroupID id) { mGroupBitset[id] = false; }
 
        /*        template <typename T, typename... TArgs>
        T& addComponent(TArgs&&... mArgs)*/
        template<typename T, typename... TArgs>
        T& addComponent(TArgs&&... args)
        {
            assert(!hasComponent<T>());
            std::unique_ptr<T> upComponent{
                std::make_unique<T>(std::forward<TArgs>(args)...)};
            upComponent->mEntity = this;
            T* pComponent = upComponent.get();
            mComponents.emplace_back(std::move(upComponent));
            mComponentArray[getComponentTypeID<T>()] = pComponent;
            mComponentBitset[getComponentTypeID<T>()] = true;
            pComponent->init();
            return *pComponent;
        }
        
        template<typename T>
        T& getComponent() const
        {
            assert(hasComponent<T>());
            auto ptr(mComponentArray[getComponentTypeID<T>()]);
            return *reinterpret_cast<T*>(ptr);
        }
    private:
        EntityManager& mManager;
        bool mIsAlive{true};
        std::vector<std::unique_ptr<Component>> mComponents;
        ComponentArray mComponentArray;
        ComponentBitset mComponentBitset;
        GroupBitset mGroupBitset;       
    };

    class EntityManager
    {
    public:
        void update(float frameTime);
        void draw();
        void addToGroup(Entity *entity, GroupID id);
        std::vector<Entity*>& getEntitiesByGroup(GroupID id);
        void refresh();
        Entity& addEntity(); 
    private:
        std::vector<std::unique_ptr<Entity>> mEntities;
        std::array<std::vector<Entity*>, maxGroups> mGroupedEntities;
    };
}

#endif /* COMPOSITION_HPP */

