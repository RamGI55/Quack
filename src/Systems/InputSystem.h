//
// Created by PC on 15-Dec-25.
//

#ifndef DUCKDUCKROAD_INPUTSYSTEM_H
#define DUCKDUCKROAD_INPUTSYSTEM_H
#include "../Core/System.h"


class Coordinator;

class InputSystem : public System
{
public:
    void init();
    void update(float dt, Coordinator& coordinator);

private:

};


#endif //DUCKDUCKROAD_INPUTSYSTEM_H