//
// Created by PC on 13-Dec-25.
//

#ifndef DUCKDUCKROAD_ENTITYMANAGER_H
#define DUCKDUCKROAD_ENTITYMANAGER_H
#include "Types.h"
#include <queue>
#include <array>

class EntityManager
{
public:
    EntityManager();
    Entity CreateEntity();
    void DestroyEntity(Entity inEntity);
    Signature GetSignature(Entity inEntity);
    void SetSignature(Entity inEntity, Signature inSignature);

private:
    std::queue<Entity> m_AvailableEntities{};

    std::array<Signature, MAX_ENTITIES> m_Signatures{};

    uint32_t m_LivingEntityCount{};
};

#endif //DUCKDUCKROAD_ENTITYMANAGER_H