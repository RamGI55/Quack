//
// Created by Hyunsoo Park on 12/25/25.
//

#ifndef MOVEMENTCOMPONENT_H
#define MOVEMENTCOMPONENT_H
#include "SFML/System/Vector2.hpp"

struct SmoothMovementComponent {

    // TODO: For free movement.
    float MovementSpeed  = 64.f;

};

struct GridMovementComponent {

public:
    float CellSize      = 64.f;
    float HopDuration   = 0.15f;

    bool isHopping      = false;
    sf::Vector2f HopStartPos;
    sf::Vector2f HopEndPos;
    float HopProgress   = 0.f;

    sf::Vector2i GridPosition;

};
#endif //MOVEMENTCOMPONENT_H
