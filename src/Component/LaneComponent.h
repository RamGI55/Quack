//
// Created by PC on 17-Jul-26.
//

#ifndef DUCKDUCKROAD_LANECOMPONENT_H
#define DUCKDUCKROAD_LANECOMPONENT_H
#include <cstddef>
#include <vector>

#include "SFML/Graphics/Texture.hpp"

enum ELaneType
{
    None    = 0,
    Cars    = 1 << 0,
    Trains  = 1 << 1,
    Water   = 1 << 2,
    Safe    = 1 << 3,
    Goal    = 1 << 4,
};

struct LaneComponent
{
    int RowIndex;
    float Direction;
    float MinSpeed;
    float MaxSpeed;
    float TimeUntilNextSpawn = 0.f;
    float LaneSize;
    ELaneType LaneType;
    std::string TextureKey;

};
#endif //DUCKDUCKROAD_LANECOMPONENT_H