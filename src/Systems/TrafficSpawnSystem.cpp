//
// Created by PC on 17-Jul-26.
//

#include "TrafficSpawnSystem.h"

#include "../Factory/EntityFactory.h"
#include "../Component/LaneComponent.h"
#include "../Core/Coordinator.h"

TrafficSpawnSystem::TrafficSpawnSystem(SystemManager* manager)
{
}

TrafficSpawnSystem::~TrafficSpawnSystem()
{
}

void TrafficSpawnSystem::Init(float WindowWith, float WindowHeight, AssetManager& assets, Coordinator& coordinator)
{
    VehicleFactory pooledVehicle(coordinator, assets);

}

void TrafficSpawnSystem::Update(const float dt, SystemManager& manager, Coordinator& coordinator)
{
    for (auto entity : Entities)
    {
        auto& LaneComponent = coordinator.GetComponent<::LaneComponent>(entity);



    }
}

bool TrafficSpawnSystem::IsOutofTraffic(Entity& entity, const float direction)
{
    if (direction == -1.0f)
    {
    }

    return false;
}

void TrafficSpawnSystem::ProcessLaneSpawn(float dt, Coordinator& coordinator, AssetManager& assets)
{
    VehicleFactory pooledVehicle(coordinator, assets);
    EntityDef EntityDef;

}
