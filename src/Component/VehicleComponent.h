//
// Created by PC on 17-Jul-26.
//

#ifndef DUCKDUCKROAD_VEHICLECOMPONENT_H
#define DUCKDUCKROAD_VEHICLECOMPONENT_H

enum EVehicleType
{
    Default  = 0,
    Small = 1 << 0,
    Train = 1 << 1,
    Log   = 1 << 2,
};

struct VehicleComponent
{
    float Speed;
    float Width;
    EVehicleType Type;
    bool bIsInLane;
    std::string soundKey;
    std::string collisionSoundKey;
};

#endif //DUCKDUCKROAD_VEHICLECOMPONENT_H