//
// Created by PC on 16-Jul-26.
//

#ifndef DUCKDUCKROAD_ANIMATIONCOMPONENT_H
#define DUCKDUCKROAD_ANIMATIONCOMPONENT_H
#include "SFML/System/Vector2.hpp"

struct AnimationComponent
{
    sf::Vector2i FrameSize   {32, 32};
    int Row                  = 0;
    int FrameCount           = 1;
    int CurrentFrame         = 0;
    float FrameDuration      = 0.1f;
    float Elapsed            = 0.f;
};

#endif //DUCKDUCKROAD_ANIMATIONCOMPONENT_H
