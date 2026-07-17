//
// Created by PC on 16-Jul-26.
//

#ifndef DUCKDUCKROAD_ANIMATIONSYSTEM_H
#define DUCKDUCKROAD_ANIMATIONSYSTEM_H
#include "../Core/System.h"

class SystemManager;
class Coordinator;

class AnimationSystem : public System
{
public:
    AnimationSystem(SystemManager* manager = nullptr);
    ~AnimationSystem();
    void Update(float dt, Coordinator& coordinator);
};

#endif //DUCKDUCKROAD_ANIMATIONSYSTEM_H
