//
// Created by PC on 13-Dec-25.
//

#ifndef DUCKDUCKROAD_INPUTCOMPONENT_H
#define DUCKDUCKROAD_INPUTCOMPONENT_H
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

struct ActionState
{
    bool Pressed    = false;
    bool Released   = false;
    bool Held       = false;
    float Value     = 0.f;
    float holdTime = 0.f;
};

enum class InputType
{
    Keyboard,
    KeyboardMouse,
    Gamepad,
};

struct InputComponent
{
    InputType inputType     = InputType::Keyboard;

    bool KeyUp        = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W);
    bool KeyDown      = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S);
    bool KeyLeft      = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A);
    bool KeyRight     = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D);
    bool KeyAction    = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Space);

    // Mouse Inputs
    ActionState Up;
    ActionState Down;
    ActionState Left;
    ActionState Right;
    ActionState Action;

    ActionState MouseTrigger1;
    ActionState MouseTrigger2;
    ActionState MouseTrigger3;

    float maxSpeed      = 200.f;
    float InputCoolTime = 0.f;

};



#endif //DUCKDUCKROAD_INPUTCOMPONENT_H