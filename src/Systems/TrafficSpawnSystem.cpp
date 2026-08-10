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

    // Remove first so a slot freed by an exiting car can be refilled in the same frame -
    // spawn always happens after the removal that made room for it.
    DespawnOffscreen(coordinator);

    std::unordered_map<int, const LaneComponent*> uniqueLanesByRow;
    for (auto entity : Entities)
    {
        auto& lane = coordinator.GetComponent<::LaneComponent>(entity);
        if (lane.LaneType != ELaneType::Cars || lane.MaxTraffics <= 0)
        {
            continue;
        }
        uniqueLanesByRow.try_emplace(lane.RowIndex, &lane);
    }

    // Shared budget across every lane this frame: never create more entities than the ECS
    // can actually hold, regardless of how high a single lane's MaxTraffics is configured.
    // This is the real ceiling for "push it to the max" - per-lane MaxTraffics just becomes
    // a request that this budget clamps down to whatever headroom is actually left.
    const uint32_t living = coordinator.GetLivingEntityCount();
    const uint32_t reserved = living + kEntitySafetyMargin;
    uint32_t budget = reserved < MAX_ENTITIES ? (MAX_ENTITIES - reserved) : 0;

    // Round-robin one spawn per lane per pass, rather than draining the whole budget into
    // whichever lane happens to be visited first - keeps traffic spread across every lane
    // even when the shared budget can't satisfy every lane's request.
    bool spawnedAny = true;
    while (budget > 0 && spawnedAny)
    {
        spawnedAny = false;
        for (auto& [row, lane] : uniqueLanesByRow)
        {
            if (budget == 0)
            {
                break;
            }
            auto& runtime = mLaneRuntimes[row];
            if (runtime.Vehicles.size() < static_cast<size_t>(lane->MaxTraffics))
            {
                runtime.Vehicles.push_back(SpawnVehicle(row, *lane, coordinator));
                --budget;
                spawnedAny = true;
            }
        }
    }

    UpdateTelemetry(dt);
}

void TrafficSpawnSystem::DespawnOffscreen(Coordinator& coordinator)
{
    mLastFrameDestroyUs = 0.0;

    // Give cars a full lane-width of slack past the edge they're exiting through before
    // destroying them. This MUST be direction-aware: spawns are staggered up to a full
    // screen-width behind their entry edge (see SpawnVehicle) so they queue up before
    // entering, and a car waiting its turn to enter is not "exited" just because its spawn
    // position is outside this band - only crossing the FAR edge (relative to its own
    // travel direction) counts. A direction-blind check here previously destroyed ~94% of
    // staggered spawns one frame after creation, before they ever reached the view.
    const float exitMargin = 128.f;

    for (auto& [row, runtime] : mLaneRuntimes)
    {
        auto& vehicles = runtime.Vehicles;
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
                // Timed narrowly around the actual delete - excludes the GetComponent read and
                // vector swap-remove above/below so this isolates Coordinator::DestroyEntity's
                // own cost (EntityManager + ComponentManager's sweep across every registered
                // component array + SystemManager's sweep across every registered system).
                const auto t0 = std::chrono::steady_clock::now();
                coordinator.DestroyEntity(vehicles[i]);
                const auto t1 = std::chrono::steady_clock::now();
                const double us = std::chrono::duration<double, std::micro>(t1 - t0).count();

                mLastFrameDestroyUs += us;
                mTotalDestroyTimeUs += us;
                mMaxDestroyTimeUs = std::max(mMaxDestroyTimeUs, us);

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

Entity TrafficSpawnSystem::SpawnVehicle(int row, const LaneComponent& lane, Coordinator& coordinator)
{
    static const std::array<const char*, 4> kArchetypes = {"car_tex", "coup_green", "coup_midnight", "coup_red"};

    const bool movingRight = lane.Direction >= 0.f;
    const float minSpeed = std::min(lane.MinSpeed, lane.MaxSpeed);
    const float maxSpeed = std::max(lane.MinSpeed, lane.MaxSpeed);

    // Stagger is bounded to one screen-width regardless of MaxTraffics - a huge per-lane cap
    // must not push spawn points hours of travel-time away from the visible view; the shared
    // entity budget (see Update) is what actually limits how many vehicles exist at once.
    std::uniform_real_distribution<float> speedDist(minSpeed, maxSpeed);
    std::uniform_real_distribution<float> staggerDist(0.f, mWorldWidth);
    std::uniform_int_distribution<size_t> texDist(0, kArchetypes.size() - 1);

    const float speed   = speedDist(mRng) * (movingRight ? 1.f : -1.f);
    const float stagger = staggerDist(mRng);
    const float spawnX  = movingRight
                         ? (-lane.LaneSize / 2.f - stagger)
                         : (mWorldWidth + lane.LaneSize / 2.f + stagger);

    EntityDef def;
    def.type       = "car";
    def.spawnX     = 0;
    def.spawnY     = row;
    def.cellSize   = lane.LaneSize;
    def.keyTexture = kArchetypes[texDist(mRng)];
    def.Velocity   = speed;

    // Timed narrowly around entity construction (5x AddComponent + a fresh sf::Sprite) -
    // the counterpart to DestroyEntity's timing in DespawnOffscreen.
    VehicleFactory factory(coordinator, *mAssets);
    const auto t0 = std::chrono::steady_clock::now();
    Entity vehicle = factory.Create(def);
    const auto t1 = std::chrono::steady_clock::now();
    const double us = std::chrono::duration<double, std::micro>(t1 - t0).count();

    mLastFrameSpawnUs += us;
    mTotalSpawnTimeUs += us;
    mMaxSpawnTimeUs = std::max(mMaxSpawnTimeUs, us);

    coordinator.GetComponent<TransformComponent>(vehicle).Position.x = spawnX;

    ++mSpawnCountThisSecond;
    ++mTotalSpawned;
    return vehicle;
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
    for (auto& [row, runtime] : mLaneRuntimes)
    {
        count += static_cast<int>(runtime.Vehicles.size());
    }
    return count;
}
