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

    sf::Keyboard::Scancode KeyUp        = sf::Keyboard::Scancode::W;
    sf::Keyboard::Scancode KeyDown      = sf::Keyboard::Scancode::S;
    sf::Keyboard::Scancode KeyLeft      = sf::Keyboard::Scancode::A;
    sf::Keyboard::Scancode KeyRight     = sf::Keyboard::Scancode::D;
    sf::Keyboard::Scancode KeyAction    = sf::Keyboard::Scancode::Space;

    // Mouse Inputs
    sf::Mouse::Button TriggerType1      = sf::Mouse::Button::Left;
    sf::Mouse::Button TriggerType2      = sf::Mouse::Button::Right;
    sf::Mouse::Button TriggerType3      = sf::Mouse::Button::Middle;

    float maxSpeed      = 200.f;
    float InputCoolTime = 0.f;

};



#endif //DUCKDUCKROAD_INPUTCOMPONENT_H