//
// Created by PC on 15-Dec-25.
//

#include "InputSystem.h"
#include <cmath>
#include "../Core/Coordinator.h"
#include "../Component/InputComponent.h"
#include "../Component/VelocityComponent.h"

void InputSystem::update(float dt, Coordinator& coordinator)
{
    for (const auto entity : Entities)
    {
        auto& input     = coordinator.GetComponent<InputComponent>(entity);
        auto& velocity  = coordinator.GetComponent<VelocityComponent>(entity);

        velocity.Velocity = { 0.f, 0.f };

        if (sf::Keyboard::isKeyPressed(input.KeyUp))
        {
            velocity.Velocity.y -= input.maxSpeed;
        }
        if (sf::Keyboard::isKeyPressed(input.KeyDown))
        {
            velocity.Velocity.y += input.maxSpeed;
        }
        if (sf::Keyboard::isKeyPressed(input.KeyRight))
        {
            velocity.Velocity.x += input.maxSpeed;
        }
        if (sf::Keyboard::isKeyPressed(input.KeyLeft))
        {
            velocity.Velocity.x -= input.maxSpeed;
        }

        // nomalising diagonal movements
        if (velocity.Velocity.x != 0.f && velocity.Velocity.y != 0.f)
        {
            float length = std::sqrt(
                velocity.Velocity.x * velocity.Velocity.x +
                velocity.Velocity.y * velocity.Velocity.y
                );
            velocity.Velocity.x = (velocity.Velocity.x / length)*input.maxSpeed;
            velocity.Velocity.y = (velocity.Velocity.y / length)*input.maxSpeed;
        }

    }
}
