//
// Created by Hyunsoo Park on 12/25/25.
//

#ifndef PAWNCOMPONENT_H
#define PAWNCOMPONENT_H



struct PawnComponent {

    Entity Onwer;
    bool isDead                 = false;
    bool isInvincible           = false;
    float InvincibilityTimer    = 0.f;
    float RespawnTimer          = 0.f;
    float LiveTimer             = 0.f;

};

#endif //PAWNCOMPONENT_H
