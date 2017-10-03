#include "composition.hpp"

ecs::Entity::Entity(EntityManager &manager):
mManager{manager}
{}

void ecs::Entity::update(float frameTime)
{
    for(auto &c: mComponents)
    {
        c->update(frameTime);
    }
}

void ecs::Entity::draw()
{
    for(auto &c: mComponents)
    {
        c->draw();
    }
}

void ecs::Entity::addGroup(GroupID id) noexcept
{
    mGroupBitset[id] = true;
    mManager.addToGroup(this, id);
}

void ecs::EntityManager::update(float frameTime)
{
    for(auto &e: mEntities)
    {
        e->update(frameTime);
    }
}

void ecs::EntityManager::draw()
{
    for(auto &e: mEntities)
    {
        e->draw();
    }
}

void ecs::EntityManager::addToGroup(Entity* entity, GroupID id)
{
    mGroupedEntities[id].emplace_back(entity);
}

std::vector<Entity*>& ecs::EntityManager::getEntitiesByGroup(GroupID id)
{
    return mGroupedEntities[id];
}

void ecs::EntityManager::refresh()
{
    for(auto i(0u); i < maxGroups; ++i)
    {
        auto& v(mGroupedEntities[i]);

        v.erase(std::remove_if(v.begin(), v.end(),
                    [i](const auto& entity)
                    {
                        return !entity->isAlive() ||
                               !entity->hasGroup(i);
                    }),
            v.end());
    }

    mEntities.erase(
        std::remove_if(mEntities.begin(), mEntities.end(),
            [](const auto& entity)
            {
                return !entity->isAlive();
            }),
        mEntities.end());
}

Entity& ecs::EntityManager::addEntity()
{
    std::unique_ptr<Entity> upEntity{std::make_unique<Entity>(*this)};
    Entity *pEntity = upEntity.get();
    mEntities.emplace_back(std::move(upEntity));
    return pEntity;
}