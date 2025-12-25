//
// Created by Hyunsoo Park on 12/25/25.
//

#include "GridMovementSystem.h"

#include "../Component/InputComponent.h"
#include "../Component/MovementComponent.h"
#include "../Component/TransformComponent.h"
#include "../Utils/MathUtils.h"

void GridMovementSystem::init() {
}

void GridMovementSystem::Update(float dt, Coordinator& coordinator) {
    for (auto entity : Entities)
    {
        auto& transformComponent    = coordinator.GetComponent<TransformComponent>(entity);
        auto& movementComponent     = coordinator.GetComponent<GridMovementComponent>(entity);
        auto& inputComponent        = coordinator.GetComponent<InputComponent>(entity);

        if (movementComponent.isHopping)
        {
            movementComponent.HopProgress += dt / movementComponent.HopDuration;
            transformComponent.Position = MathUtils::Lerp(movementComponent.HopStartPos, movementComponent.HopEndPos, movementComponent.HopProgress);
            if (movementComponent.HopProgress >= 1.0f)
            {
                // snap the hoppingPos.
                transformComponent.Position = movementComponent.HopEndPos;
                movementComponent.isHopping = false;
            }
        }
        else
        {
            sf::Vector2i direction = { 0, 0 };

            // Reading the input
            if (sf::Keyboard::isKeyPressed(inputComponent.KeyUp)) {
                direction.y -= 1.f;
            }
            if (sf::Keyboard::isKeyPressed(inputComponent.KeyDown)) {
                direction.y += 1.f;
            }
            if (sf::Keyboard::isKeyPressed(inputComponent.KeyLeft)) {
                direction.x -= 1.f;
            }
            if (sf::Keyboard::isKeyPressed(inputComponent.KeyRight)) {
                direction.x += 1.f;
            }

            if (direction.x != 0 || direction.y != 0) {
                movementComponent.HopStartPos   = transformComponent.Position;
                movementComponent.GridPosition  += direction;
                movementComponent.HopEndPos     = MathUtils::GridToWorld(movementComponent.GridPosition, movementComponent.CellSize);
                movementComponent.HopProgress   = 0.0f;
                movementComponent.isHopping     = true;
            }

        }
    }

}
