//
// Created by PC on 13-Dec-25.
//

#include "EntityManager.h"
#include <cassert>

EntityManager::EntityManager()
{
    // iterating the all entities. using uint_32 to matching as the type of the entity.
    for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
    {
        AvailableEntities_.push(entity);
    }
}

Entity EntityManager::CreateEntity()
{
    assert(LivingEntityCount_ < MAX_ENTITIES && "Too many entities");

    Entity id = AvailableEntities_.front();
    AvailableEntities_.pop();
    ++LivingEntityCount_;

    return id;
}

void EntityManager::DestroyEntity(Entity inEntity)
{
    assert(LivingEntityCount_ > 0 && "No Entities to delete");

    AvailableEntities_.push(inEntity);
    --LivingEntityCount_;
}

Signature EntityManager::GetSignature(Entity inEntity)
{
    return Signatures_[inEntity];
}

void EntityManager::SetSignature(Entity inEntity, Signature inSignature)
{
    Signatures_[inEntity] = inSignature;
}

