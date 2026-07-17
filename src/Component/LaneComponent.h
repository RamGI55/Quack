//
// Created by PC on 17-Jul-26.
//

#ifndef DUCKDUCKROAD_LANECOMPONENT_H
#define DUCKDUCKROAD_LANECOMPONENT_H
#include <cstddef>
#include <vector>

struct LaneComponent
{
    float MinSpeed;
    float MaxSpeed;
    float TimeUntilNextSpawn = 0.f;
};
wh
#endif //DUCKDUCKROAD_LANECOMPONENT_H