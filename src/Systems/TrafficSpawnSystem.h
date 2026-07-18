//
// Created by PC on 17-Jul-26.
//

#ifndef DUCKDUCKROAD_TRAFFICSPAWNSYSTEM_H
#define DUCKDUCKROAD_TRAFFICSPAWNSYSTEM_H
#include <vector>

#include "../Core/System.h"


class AssetManager;
class Coordinator;
class SystemManager;

class TrafficSpawnSystem : public System
{
public:
    TrafficSpawnSystem(SystemManager* manager = nullptr);
    ~TrafficSpawnSystem() override;
    void Init(float WindowWith, float WindowHeight, AssetManager& assets, Coordinator& coordinator);
    void Update(const float dt, SystemManager& manager, Coordinator& coordinator);

    bool IsOutofTraffic(Entity& entity, const float direction);

    void DeactivateLaneMovement(Coordinator& coordinator);
    void ActivateLaneMovement(Coordinator& coordinator);
    void ProcessLaneSpawn(float dt, Coordinator& coordinator, AssetManager& assets);

private:
    std::vector<Entity> OnReadyTraffics;
};


#endif //DUCKDUCKROAD_TRAFFICSPAWNSYSTEM_H