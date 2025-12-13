//
// Created by PC on 13-Dec-25.
//

#ifndef DUCKDUCKROAD_INPUTCOMPONENT_H
#define DUCKDUCKROAD_INPUTCOMPONENT_H
#include <SFML/Window/Keyboard.hpp>

struct ActionState
{
    bool Pressed    = false;
    bool Released   = false;
    bool Held       = false;
    float Value     = 0.f;
};

struct InputComponent
{
    sf::Keyboard::Scancode KeyUp        = sf::Keyboard::Scancode::W;
    sf::Keyboard::Scancode KeyDown      = sf::Keyboard::Scancode::S;
    sf::Keyboard::Scancode KeyLeft      = sf::Keyboard::Scancode::A;
    sf::Keyboard::Scancode KeyRight     = sf::Keyboard::Scancode::D;
    sf::Keyboard::Scancode KeyAction    = sf::Keyboard::Scancode::Space;
};



#endif //DUCKDUCKROAD_INPUTCOMPONENT_H