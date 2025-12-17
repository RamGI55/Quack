//
// Created by PC on 14-Dec-25.
//

#ifndef DUCKDUCKROAD_COLLISIONCOMPONENT_H
#define DUCKDUCKROAD_COLLISIONCOMPONENT_H
#include "SFML/Graphics/RectangleShape.hpp"

struct CollisionComponent
{
    sf::FloatRect   CollisionBody;
    bool isTrigger  = false;
    bool isStatic   = false;
    bool visualised = false;
    uint32_t layer  = 0;

};
#endif //DUCKDUCKROAD_COLLISIONCOMPONENT_H