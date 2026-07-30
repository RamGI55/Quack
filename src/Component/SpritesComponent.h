//
// Created by PC on 14-Dec-25.
//

#ifndef DUCKDUCKROAD_SPRITESCOMPONENT_H
#define DUCKDUCKROAD_SPRITESCOMPONENT_H
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"

struct SpriteComponent
{
    std::optional<sf::Sprite> Sprite;
    std::shared_ptr<sf::Texture> Texture;
    sf::IntRect rect;
    sf::Vector2f Origin;
    uint8_t layer{0};
};

#endif //DUCKDUCKROAD_SPRITESCOMPONENT_H