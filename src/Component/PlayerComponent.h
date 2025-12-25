//
// Created by PC on 14-Dec-25.
//

#ifndef DUCKDUCKROAD_PLAYERCOMPONENT_H
#define DUCKDUCKROAD_PLAYERCOMPONENT_H
#include <cstdint>

#include "../Core/Types.h"

struct PlayerComponent
{
    uint32_t PlayerId;
    std::optional<Entity> PossessedPawn;

};
#endif //DUCKDUCKROAD_PLAYERCOMPONENT_H