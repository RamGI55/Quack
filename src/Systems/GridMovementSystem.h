//
// Created by Hyunsoo Park on 12/25/25.
//

#ifndef GRIDMOVEMENTSYSTEM_H
#define GRIDMOVEMENTSYSTEM_H
#include "../Core/Coordinator.h"
#include "../Core/System.h"


class GridMovementSystem : public System{
public:
    GridMovementSystem(SystemManager* manager) {}
    void init();
    void Update(float dt, Coordinator& coordinator);
};


#endif //GRIDMOVEMENTSYSTEM_H
