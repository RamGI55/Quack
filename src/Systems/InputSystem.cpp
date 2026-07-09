//
// Created by PC on 15-Dec-25.
//

#include "InputSystem.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include "../Core/Coordinator.h"
#include "../Component/InputComponent.h"

InputSystem::InputSystem(SystemManager* manager)
{
}

InputSystem::~InputSystem()
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

        // Using SFML input interface - change custom multiplatform input interface later.
        UpdateState(input.Up, sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W), dt);
        UpdateState(input.Down, sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S), dt);
        UpdateState(input.Left, sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A), dt);
        UpdateState(input.Right, sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D) , dt);
        UpdateState(input.Action, sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Space), dt);

        UpdateState(input.MouseTrigger1, sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) , dt);
        UpdateState(input.MouseTrigger1, sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) , dt);
        UpdateState(input.MouseTrigger1, sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle) , dt);

    }
}
