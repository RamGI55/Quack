//
// Created by PC on 15-Dec-25.
//

#include "RenderSystem.h"

#include "../Component/SpritesComponent.h"
#include "../Component/TransformComponent.h"
#include "../Core/Coordinator.h"

RenderSystem::RenderSystem(SystemManager* manager)
{
}

RenderSystem::~RenderSystem()
{
}

void RenderSystem::init(const std::shared_ptr<sf::Texture>& inTexture, Coordinator& coordinator) {
    for (auto& entity : Entities) {
        auto& sprites   = coordinator.GetComponent<SpriteComponent>(entity);
        sprites.Texture = inTexture;
    }
}

void RenderSystem::update(sf::RenderWindow& window, Coordinator &coordinator) {
    for (auto& entity : Entities) {
        auto& sprites   = coordinator.GetComponent<SpriteComponent>(entity);
        auto& transform     = coordinator.GetComponent<TransformComponent>(entity);

        if (sprites.Sprite)
        {
            // Only READ transform/sprite data, apply to sf::Sprite for drawing
            sprites.Sprite->setPosition(transform.Position);
            sprites.Sprite->setRotation(transform.Angle);
            sprites.Sprite->setScale(transform.Scale);
            sprites.Sprite->setTextureRect(sprites.rect);

            window.draw(*sprites.Sprite);
        }
    }
}
