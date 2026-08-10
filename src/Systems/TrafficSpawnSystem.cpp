//
// Created by PC on 17-Jul-26.
//

#include "TrafficSpawnSystem.h"

#include <algorithm>
#include <array>
#include <chrono>

#include "../Component/LaneComponent.h"
#include "../Component/TransformComponent.h"
#include "../Component/VelocityComponent.h"
#include "../Core/Coordinator.h"
#include "../Core/Types.h"
#include "../Factory/EntityFactory.h"

namespace
{
    // Headroom kept below MAX_ENTITIES so CreateEntity's own strict "<" assert is never
    // the thing that gets hit even if something outside this system creates an entity
    // between our budget check and the actual spawn.
    constexpr uint32_t kEntitySafetyMargin = 16;

    // How long the gap between two cars must take to close, at the lane's own top speed,
    // before that gap counts as crossable. Distance alone isn't enough - a fast lane needs
    // more physical space than a slow one to offer the same window to get through.
    constexpr float kMinCrossableGapSeconds = 1.0f;

    constexpr std::array<const char*, 4> kVehicleArchetypes =
        {"car_tex", "coup_green", "coup_midnight", "coup_red"};
}

TrafficSpawnSystem::TrafficSpawnSystem(SystemManager* manager)
{
}

TrafficSpawnSystem::~TrafficSpawnSystem()
{
}

void TrafficSpawnSystem::Init(float WorldWidth, float WorldHeight, AssetManager& assets, Coordinator& coordinator)
{
    mWorldWidth  = WorldWidth;
    mWorldHeight = WorldHeight;
    mAssets      = &assets;
}

void TrafficSpawnSystem::Update(float dt, Coordinator& coordinator)
{
    mLastFrameSpawnUs = 0.0;

    if (!mPoolFilled)
    {
        FillPool(coordinator);
        mPoolFilled = true;
    }

    // Recycle first so a slot freed by an exiting car can be refilled in the same frame.
    RecycleOffscreen(coordinator);
    RefillLanes(coordinator);

    UpdateTelemetry(dt);
}

std::unordered_map<int, const LaneComponent*> TrafficSpawnSystem::CollectCarLanesByRow(Coordinator& coordinator) const
{
    std::unordered_map<int, const LaneComponent*> lanesByRow;
    for (auto entity : Entities)
    {
        auto& lane = coordinator.GetComponent<::LaneComponent>(entity);
        if (lane.LaneType != ELaneType::Cars || lane.MaxTraffics <= 0)
        {
            continue;
        }
        lanesByRow.try_emplace(lane.RowIndex, &lane);
    }
    return lanesByRow;
}

int TrafficSpawnSystem::GetLaneCapacity(const LaneComponent& lane) const
{
    if (mStressTestUncapped)
    {
        return lane.MaxTraffics;
    }

    const float maxSpeed   = std::max(lane.MinSpeed, lane.MaxSpeed);
    const float slotWidth  = lane.LaneSize + maxSpeed * kMinCrossableGapSeconds;
    const int   maxFit     = static_cast<int>(mWorldWidth / slotWidth);
    return std::min(lane.MaxTraffics, std::max(maxFit, 1));
}

void TrafficSpawnSystem::FillPool(Coordinator& coordinator)
{
    const auto lanesByRow = CollectCarLanesByRow(coordinator);

    // Same ceiling as before, just checked once here instead of every frame - after this,
    // the pool never grows, so there's nothing left to re-check.
    const uint32_t living = coordinator.GetLivingEntityCount();
    uint32_t budget = (living + kEntitySafetyMargin < MAX_ENTITIES) ? (MAX_ENTITIES - living - kEntitySafetyMargin) : 0;

    // Kept within a fraction of the slot so cars never overlap or cross into a neighbour's
    // slot - the even spacing (and therefore GetLaneCapacity's crossable gap) stays intact.
    std::uniform_real_distribution<float> jitterDist(-0.2f, 0.2f);
    std::unordered_map<int, int> placedPerLane;

    // Round-robin one vehicle per lane per pass rather than draining the whole budget into
    // whichever lane is visited first - matters even more than it used to now that a single
    // lane's requested MaxTraffics can exceed the entire shared budget (stress mode).
    bool placedAny = true;
    while (budget > 0 && placedAny)
    {
        placedAny = false;
        for (auto& [row, lane] : lanesByRow)
        {
            if (budget == 0)
            {
                break;
            }

            const int capacity = GetLaneCapacity(*lane);
            int& placed = placedPerLane[row];
            if (placed >= capacity)
            {
                continue;
            }

            const float slot = mWorldWidth / static_cast<float>(capacity);
            const float x    = slot * static_cast<float>(placed) + slot / 2.f + jitterDist(mRng) * slot;

            Entity vehicle = ConstructVehicle(row, *lane, coordinator);
            coordinator.GetComponent<TransformComponent>(vehicle).Position.x = x;
            mLaneRuntimes[row].push_back(vehicle);
            ++placed;
            --budget;
            placedAny = true;
        }
    }
}

void TrafficSpawnSystem::RecycleOffscreen(Coordinator& coordinator)
{
    // Give cars a full lane-width of slack past the edge they're exiting through before
    // recycling them. This MUST be direction-aware: a car waiting its turn to enter is not
    // "exited" just because its position is outside this band - only crossing the FAR edge
    // (relative to its own travel direction) counts. A direction-blind check here previously
    // recycled ~94% of staggered spawns one frame after creation, before they ever reached
    // the view.
    const float exitMargin = 128.f;

    for (auto& [row, vehicles] : mLaneRuntimes)
    {
        for (size_t i = 0; i < vehicles.size();)
        {
            auto& transform = coordinator.GetComponent<TransformComponent>(vehicles[i]);
            auto& velocity  = coordinator.GetComponent<VelocityComponent>(vehicles[i]);
            const bool movingRight = velocity.Velocity.x >= 0.f;
            const bool hasExited = movingRight
                                  ? (transform.Position.x > mWorldWidth + exitMargin)
                                  : (transform.Position.x < -exitMargin);

            if (hasExited)
            {
                mReserve.push_back(vehicles[i]);
                vehicles[i] = vehicles.back();
                vehicles.pop_back();
                ++mDespawnCountThisSecond;
                ++mTotalDespawned;
            }
            else
            {
                ++i;
            }
        }
    }
}

void TrafficSpawnSystem::RefillLanes(Coordinator& coordinator)
{
    const auto lanesByRow = CollectCarLanesByRow(coordinator);

    // Same round-robin reasoning as FillPool: with the crossability clamp off, a single
    // lane's target can exceed the entire reserve, so fill breadth-first across lanes.
    bool placedAny = true;
    while (!mReserve.empty() && placedAny)
    {
        placedAny = false;
        for (auto& [row, lane] : lanesByRow)
        {
            if (mReserve.empty())
            {
                break;
            }

            auto& vehicles = mLaneRuntimes[row];
            const size_t target = static_cast<size_t>(GetLaneCapacity(*lane));
            if (vehicles.size() >= target)
            {
                continue;
            }

            Entity vehicle = mReserve.back();
            mReserve.pop_back();
            PlaceVehicleAtEntry(vehicle, row, *lane, coordinator);
            vehicles.push_back(vehicle);
            placedAny = true;
        }
    }
}

Entity TrafficSpawnSystem::ConstructVehicle(int row, const LaneComponent& lane, Coordinator& coordinator)
{
    const bool movingRight = lane.Direction >= 0.f;
    const float minSpeed = std::min(lane.MinSpeed, lane.MaxSpeed);
    const float maxSpeed = std::max(lane.MinSpeed, lane.MaxSpeed);

    std::uniform_real_distribution<float> speedDist(minSpeed, maxSpeed);
    std::uniform_int_distribution<size_t> texDist(0, kVehicleArchetypes.size() - 1);

    const float speed = speedDist(mRng) * (movingRight ? 1.f : -1.f);

    EntityDef def;
    def.type       = "car";
    def.spawnX     = 0;
    def.spawnY     = row;
    def.cellSize   = lane.LaneSize;
    def.keyTexture = kVehicleArchetypes[texDist(mRng)];
    def.Velocity   = speed;

    // Timed narrowly around entity construction (5x AddComponent + a fresh sf::Sprite).
    // Only ever runs here, during the one-time pool fill - never again once recycling starts.
    VehicleFactory factory(coordinator, *mAssets);
    const auto t0 = std::chrono::steady_clock::now();
    Entity vehicle = factory.Create(def);
    const auto t1 = std::chrono::steady_clock::now();
    const double us = std::chrono::duration<double, std::micro>(t1 - t0).count();

    mLastFrameSpawnUs += us;
    mTotalSpawnTimeUs += us;
    mMaxSpawnTimeUs = std::max(mMaxSpawnTimeUs, us);

    ++mSpawnCountThisSecond;
    ++mTotalSpawned;
    return vehicle;
}

void TrafficSpawnSystem::PlaceVehicleAtEntry(Entity vehicle, int row, const LaneComponent& lane, Coordinator& coordinator)
{
    const bool movingRight = lane.Direction >= 0.f;
    const float minSpeed = std::min(lane.MinSpeed, lane.MaxSpeed);
    const float maxSpeed = std::max(lane.MinSpeed, lane.MaxSpeed);

    std::uniform_real_distribution<float> speedDist(minSpeed, maxSpeed);
    const float speed  = speedDist(mRng) * (movingRight ? 1.f : -1.f);
    const float spawnX = movingRight ? (-lane.LaneSize / 2.f) : (mWorldWidth + lane.LaneSize / 2.f);
    const float spawnY = row * lane.LaneSize + lane.LaneSize / 2.f;

    auto& transform = coordinator.GetComponent<TransformComponent>(vehicle);
    transform.Position = {spawnX, spawnY};
    transform.Angle     = sf::degrees(movingRight ? 90.f : -90.f);

    coordinator.GetComponent<VelocityComponent>(vehicle).Velocity = {speed, 0.f};
}

void TrafficSpawnSystem::UpdateTelemetry(float dt)
{
    mTelemetryTimer += dt;
    if (mTelemetryTimer >= 1.f)
    {
        mLastSpawnsPerSecond   = mSpawnCountThisSecond;
        mLastDespawnsPerSecond = mDespawnCountThisSecond;
        mSpawnCountThisSecond   = 0;
        mDespawnCountThisSecond = 0;
        mTelemetryTimer -= 1.f;
    }
}

int TrafficSpawnSystem::GetActiveVehicleCount() const
{
    int count = 0;
    for (auto& [row, vehicles] : mLaneRuntimes)
    {
        count += static_cast<int>(vehicles.size());
    }
    return count;
}
