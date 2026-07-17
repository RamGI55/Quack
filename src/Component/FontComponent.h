//
// Created by PC on 17-Jul-26.
//

#ifndef DUCKDUCKROAD_FONTCOMPONENT_H
#define DUCKDUCKROAD_FONTCOMPONENT_H
#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/Font.hpp"

struct FontComponent
{
    sf::Font font;
    sf::Color textColor;
    sf::Vector2f position;
};

#endif //DUCKDUCKROAD_FONTCOMPONENT_H