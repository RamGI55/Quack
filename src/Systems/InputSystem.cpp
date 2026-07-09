//
// Created by PC on 15-Dec-25.
//

#include "InputSystem.h"
#include <cmath>
#include "../Core/Coordinator.h"
#include "../Component/InputComponent.h"

InputSystem::InputSystem(SystemManager* manager)
{
}

void InputSystem::init()
{
}

void InputSystem::UpdateState(ActionState& state, bool down, float dt)
{
    if (down)
    {
        state.holdTime += dt;
    }
    else
    {
        state.holdTime = 0.f;
    }

    state.Pressed = down && !state.Held;
    state.Released = !down && state.Held;
    state.Held = state.holdTime > 0.5;
    state.Value = down ? 1.f : 0.f;

}

void InputSystem::Update(float dt, Coordinator& coordinator)
{
    for (const auto entity : Entities)
    {
        auto& input     = coordinator.GetComponent<InputComponent>(entity);

        UpdateState(input.Up, input.KeyUp, dt);
        UpdateState(input.Down, input.KeyDown, dt);
        UpdateState(input.Left, input.KeyLeft, dt);
        UpdateState(input.Right, input.KeyRight, dt);
        UpdateState(input.Action, input.KeyAction, dt);

        // nomalising diagonal movements
        /*if (velocity.Velocity.x != 0.f && velocity.Velocity.y != 0.f)
        {
            float length = std::sqrt(
                velocity.Velocity.x * velocity.Velocity.x +
                velocity.Velocity.y * velocity.Velocity.y
                );
            velocity.Velocity.x = (velocity.Velocity.x / length)*input.maxSpeed;
            velocity.Velocity.y = (velocity.Velocity.y / length)*input.maxSpeed;
        }*/

    }
}
