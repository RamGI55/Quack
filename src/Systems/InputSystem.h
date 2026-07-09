//
// Created by PC on 15-Dec-25.
//

#ifndef DUCKDUCKROAD_INPUTSYSTEM_H
#define DUCKDUCKROAD_INPUTSYSTEM_H

#include "../Core/System.h"


enum class InputType;
class SystemManager;
struct ActionState;
class Coordinator;

class InputSystem : public System
{
public:
    InputSystem(SystemManager* manager);
    ~InputSystem();
    void init();
    void UpdateState(ActionState& state, bool down, float dt);
    void Update(float dt, Coordinator& coordinator);

private:

};


#endif //DUCKDUCKROAD_INPUTSYSTEM_H