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

    // TODO: move the system class.
    void setTexture(const std::shared_ptr<sf::Texture>& newTexture)
    {
        Texture = newTexture;
        Sprite.emplace(*Texture);
        rect = sf::IntRect({0, 0}, sf::Vector2i(Texture->getSize()));
    }

};

#endif //DUCKDUCKROAD_SPRITESCOMPONENT_H