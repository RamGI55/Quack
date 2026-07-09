//
// Created by PC on 13-Dec-25.
//

#ifndef DUCKDUCKROAD_INPUTCOMPONENT_H
#define DUCKDUCKROAD_INPUTCOMPONENT_H


struct ActionState
{
    bool IsBlocked  = false;
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