//
// Created by PC on 15-Dec-25.
//

#include "RenderSystem.h"

#include "../Component/SpritesComponent.h"
#include "../Component/TransformComponent.h"
#include "../Core/Coordinator.h"

void RenderSystem::init(std::shared_ptr<sf::Texture> inTexture, Coordinator& coordinator) {
    for (auto& entity : Entities) {
        auto& sprites   = coordinator.GetComponent<SpriteComponent>(entity);
        sprites.setTexture(inTexture);
    }
}

void RenderSystem::update(float dt, Coordinator &coordinator) {
    for (auto& entity : Entities) {
        auto& sprites   = coordinator.GetComponent<SpriteComponent>(entity);
        auto& transform     = coordinator.GetComponent<TransformComponent>(entity);

    }
}
