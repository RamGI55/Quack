//
// Created by PC on 15-Dec-25.
//

#include "MovementSystem.h"
#include "../Core/Coordinator.h"
#include "../Component/TransformComponent.h"
#include "../Component/VelocityComponent.h"

void MovementSystem::update(const float dt, Coordinator& coordinator)
{
    for (auto entity: Entities)
    {
        auto& transform     = coordinator.GetComponent<TransformComponent>(entity);
        auto& velocity      = coordinator.GetComponent<VelocityComponent>(entity);

        transform.Position += velocity.Velocity * dt;
    }
}
