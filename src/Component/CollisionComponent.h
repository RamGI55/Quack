//
// Created by PC on 14-Dec-25.
//

#ifndef DUCKDUCKROAD_COLLISIONCOMPONENT_H
#define DUCKDUCKROAD_COLLISIONCOMPONENT_H
#include "SFML/Graphics/RectangleShape.hpp"


struct AABBCollisionComponent
{
    sf::FloatRect   CollisionRect; // using rect shape for collision
    uint32_t Layer  = 0;
    uint32_t Mask   = 0;
    bool IsTrigger  = false;
    bool IsStatic   = false;
    bool Visualised = false;


};

#endif //DUCKDUCKROAD_COLLISIONCOMPONENT_H