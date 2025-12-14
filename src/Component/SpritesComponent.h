//
// Created by PC on 14-Dec-25.
//

#ifndef DUCKDUCKROAD_SPRITESCOMPONENT_H
#define DUCKDUCKROAD_SPRITESCOMPONENT_H
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"

struct SpriteComponent
{
    std::optional<sf::Sprite> Sprite;  // Caution: sf::Sprite does not have constructor.
    std::shared_ptr<sf::Texture> Texture;
    int layer{0};

    void setTexture(const std::shared_ptr<sf::Texture>& newTexture)
    {
        Texture = newTexture;
        Sprite.emplace(*Texture);
    }
};


#endif //DUCKDUCKROAD_SPRITESCOMPONENT_H