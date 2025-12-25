//
// Created by Hyunsoo Park on 12/25/25.
//

#ifndef PAWNCOMPONENT_H
#define PAWNCOMPONENT_H
#include "../Core/Types.h"


struct PawnComponent {

    std::optional<Entity> Owner;
    bool isDead                 = false;
    bool isInvincible           = false;
    float InvincibilityTimer    = 0.f;
    float RespawnTimer          = 0.f;
    float AliveTimer             = 0.f;

};

#endif //PAWNCOMPONENT_H
