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
        m_AvailableEntities.push(entity);
    }
}

Entity EntityManager::CreateEntity()
{
    assert(m_LivingEntityCount < MAX_ENTITIES && "Too many entities");

    Entity id = m_AvailableEntities.front();
    m_AvailableEntities.pop();
    ++m_LivingEntityCount;

    return id;
}

void EntityManager::DestroyEntity(Entity inEntity)
{
    assert(m_LivingEntityCount > 0 && "No Entities to delete");
    m_Signatures[inEntity].reset();

    m_AvailableEntities.push(inEntity);
    --m_LivingEntityCount;
}

Signature EntityManager::GetSignature(Entity inEntity)
{
    return m_Signatures[inEntity];
}

void EntityManager::SetSignature(Entity inEntity, Signature inSignature)
{
    m_Signatures[inEntity] = inSignature;
}

