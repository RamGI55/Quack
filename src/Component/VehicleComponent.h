//
// Created by PC on 17-Jul-26.
//

#ifndef DUCKDUCKROAD_VEHICLECOMPONENT_H
#define DUCKDUCKROAD_VEHICLECOMPONENT_H

enum EVehicleType
{
    Default,
    Small,
    Train,
    Log
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