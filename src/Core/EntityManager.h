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
    std::queue<Entity> AvailableEntities_{};

    std::array<Signature, MAX_ENTITIES> Signatures_{};

    uint32_t LivingEntityCount_{};
};

#endif //DUCKDUCKROAD_ENTITYMANAGER_H