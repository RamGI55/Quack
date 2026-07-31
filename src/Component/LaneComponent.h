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
    None,
    Cars,
    Trains,
    Water,
    Safe,
    Goal
};

struct LaneComponent
{
    int RowIndex;
    int MaxTraffics;
    float Direction;
    float MinSpeed;
    float MaxSpeed;
    float LaneSize = 64.0f;
    ELaneType LaneType;
    std::string TextureKey;
};
#endif //DUCKDUCKROAD_LANECOMPONENT_H