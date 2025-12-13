//
// Created by PC on 13-Dec-25.
//

#ifndef DUCKDUCKROAD_TRANSFORMCOMPONENT_H
#define DUCKDUCKROAD_TRANSFORMCOMPONENT_H
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Angle.hpp>

struct TransformComponent
{
    sf::Vector2f    Position{0.f, 0.f};
    sf::Angle       Angle{sf::degrees(0)};
    sf::Vector2f    Scale{1.f, 1.f};
};




#endif //DUCKDUCKROAD_TRANSFORMCOMPONENT_H