//
// Created by Hyunsoo Park on 12/25/25.
//

#include "GridMovementSystem.h"

#include <cmath>

#include "../Component/AnimationComponent.h"
#include "../Component/InputComponent.h"
#include "../Component/MovementComponent.h"
#include "../Component/TransformComponent.h"
#include "../Utils/MathUtils.h"

namespace
{
    // duck-Sheet.png rows (see resources/Duck/duck-Sheet.png, 32x32 cells):
    // row 0 = side-profile run cycle, facing right, 24 frames - used for Left/Right (mirrored for Left).
    // row 2 = front-facing idle pose, 8 frames - played when there is no directional input.
    // row 4 = run_back, 10 frames - used for both Up and Down (no dedicated up/down art).
    constexpr int kSideRunRow        = 0;
    constexpr int kSideRunFrameCount = 24;
    constexpr int kIdleRow           = 2;
    constexpr int kIdleFrameCount    = 8;
    constexpr int kBackRow           = 4;
    constexpr int kBackFrameCount    = 10;
}

void GridMovementSystem::init() {
}

void GridMovementSystem::ResetToSpawn(Entity entity, Coordinator& coordinator)
{
    auto& movement  = coordinator.GetComponent<GridMovementComponent>(entity);
    auto& transform = coordinator.GetComponent<TransformComponent>(entity);

    movement.GridPosition = movement.SpawnGridPosition;
    movement.isHopping    = false;
    movement.HopProgress  = 0.f;
    transform.Position    = MathUtils::GridToWorld(movement.SpawnGridPosition, movement.CellSize);
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
            if (inputComponent.Up.Value > 0.0f) {
                direction.y -= 1.f;
            }
            if (inputComponent.Down.Value > 0.0f) {
                direction.y += 1.f;
            }
            if (inputComponent.Left.Value > 0.0f) {
                direction.x -= 1.f;
            }
            if (inputComponent.Right.Value > 0.0f) {
                direction.x += 1.f;
            }

            if (direction.x != 0 || direction.y != 0) {
                movementComponent.HopStartPos   = transformComponent.Position;
                movementComponent.GridPosition  += direction;
                movementComponent.HopEndPos     = MathUtils::GridToWorld(movementComponent.GridPosition, movementComponent.CellSize);
                movementComponent.HopProgress   = 0.0f;
                movementComponent.isHopping     = true;

                auto& anim = coordinator.GetComponent<AnimationComponent>(entity);
                if (direction.x != 0) {
                    anim.Row        = kSideRunRow;
                    anim.FrameCount = kSideRunFrameCount;
                    // Art only faces right - mirror the sprite for Left via the transform's scale sign.
                    transformComponent.Scale.x = std::abs(transformComponent.Scale.x) * (direction.x > 0 ? 1.f : -1.f);
                } else {
                    anim.Row        = kBackRow;
                    anim.FrameCount = kBackFrameCount;
                }
                anim.CurrentFrame = 0;
                anim.Elapsed      = 0.f;
            } else {
                auto& anim = coordinator.GetComponent<AnimationComponent>(entity);
                if (anim.Row != kIdleRow) {
                    anim.Row          = kIdleRow;
                    anim.FrameCount   = kIdleFrameCount;
                    anim.CurrentFrame = 0;
                    anim.Elapsed      = 0.f;
                }
            }

        }
    }

}
