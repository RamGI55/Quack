//
// Created by PC on 15-Dec-25.
//

#ifndef DUCKDUCKROAD_MOVEMENTSYSTEM_H
#define DUCKDUCKROAD_MOVEMENTSYSTEM_H
#include "../Core/System.h"


class Coordinator;

class MovementSystem : public System
{
public:
    void init();
    void update(float dt, Coordinator& coordinator);
};


#endif //DUCKDUCKROAD_MOVEMENTSYSTEM_H