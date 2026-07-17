//
// Created by PC on 15-Dec-25.
//

#ifndef DUCKDUCKROAD_MOVEMENTSYSTEM_H
#define DUCKDUCKROAD_MOVEMENTSYSTEM_H
#include "../Core/System.h"
#include "../Core/SystemManager.h"


class Coordinator;

class MovementSystem : public System
{
public:
    MovementSystem(SystemManager* manager = nullptr);
    void init();
    void Update(float dt, Coordinator& coordinator);
};


#endif //DUCKDUCKROAD_MOVEMENTSYSTEM_H