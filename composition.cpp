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

std::vector<ecs::Entity*>& ecs::EntityManager::getEntitiesByGroup(GroupID id)
{
    return mGroupedEntities[id];
}

/*        void refresh()
        {
            for(auto i(0u); i < maxGroups; ++i)
            {
                auto& v(groupedEntities[i]);

                v.erase(std::remove_if(std::begin(v), std::end(v),
                            [i](Entity* mEntity)
                            {
                                return !mEntity->isAlive() ||
                                       !mEntity->hasGroup(i);
                            }),
                    std::end(v));
            }

            entities.erase(
                std::remove_if(std::begin(entities), std::end(entities),
                    [](const std::unique_ptr<Entity>& mEntity)
                    {
                        return !mEntity->isAlive();
                    }),
                std::end(entities));
        }*/

void ecs::EntityManager::refresh()
{
    for(auto i(0u); i < maxGroups; ++i)
    {
        auto& v(mGroupedEntities[i]);

        v.erase(std::remove_if(std::begin(v), std::end(v),
                    [i](Entity* entity)
                    {
                        return !entity->isAlive() ||
                               !entity->hasGroup(i);
                    }),
            std::end(v));
    }

    mEntities.erase(
        std::remove_if(mEntities.begin(), mEntities.end(),
            [](const auto& entity)
            {
                return !entity->isAlive();
            }),
        mEntities.end());
}

ecs::Entity& ecs::EntityManager::addEntity()
{
    std::unique_ptr<Entity> upEntity{std::make_unique<Entity>(*this)};
    Entity *pEntity = upEntity.get();
    mEntities.emplace_back(std::move(upEntity));
    return *pEntity;
}