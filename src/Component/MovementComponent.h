//
// Created by Hyunsoo Park on 12/25/25.
//

#ifndef MOVEMENTCOMPONENT_H
#define MOVEMENTCOMPONENT_H
#include "SFML/System/Vector2.hpp"

struct SmoothMovementComponent
{
    float MovementSpeed  = 64.f;
};

struct GridMovementComponent
{
    float CellSize      = 64.f;
    float HopDuration   = 0.15f;

    bool isHopping      = false;
    sf::Vector2f HopStartPos;
    sf::Vector2f HopEndPos;
    float HopProgress   = 0.f;

    sf::Vector2i GridPosition;
    sf::Vector2i SpawnGridPosition;
};
#endif //MOVEMENTCOMPONENT_H
