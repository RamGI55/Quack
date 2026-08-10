//
// Created by PC on 17-Jul-26.
//

#ifndef DUCKDUCKROAD_TRAFFICSPAWNSYSTEM_H
#define DUCKDUCKROAD_TRAFFICSPAWNSYSTEM_H
#include <random>
#include <unordered_map>
#include <vector>

#include "../Core/System.h"


class AssetManager;
class Coordinator;
class SystemManager;
struct LaneComponent;

class TrafficSpawnSystem : public System
{
public:
    TrafficSpawnSystem(SystemManager* manager = nullptr);
    ~TrafficSpawnSystem() override;

    void Init(float WorldWidth, float WorldHeight, AssetManager& assets, Coordinator& coordinator);
    void Update(float dt, Coordinator& coordinator);

    // Stress mode: GetLaneCapacity() returns LaneComponent::MaxTraffics as-is, skipping the
    // crossability clamp - the shared entity budget in FillPool becomes the only ceiling,
    // same as the pre-pooling stress test. Off by default (real play wants crossable lanes).
    void SetStressTestUncapped(bool enabled) { mStressTestUncapped = enabled; }

    // Stress-test telemetry, read by the HUD.
    [[nodiscard]] int GetActiveVehicleCount() const;
    [[nodiscard]] int GetSpawnsPerSecond() const { return mLastSpawnsPerSecond; }
    [[nodiscard]] int GetDespawnsPerSecond() const { return mLastDespawnsPerSecond; }

    // Cumulative totals for the whole run, read once at shutdown (see Game::PrintDebugSummary).
    [[nodiscard]] uint64_t GetTotalSpawned() const { return mTotalSpawned; }
    [[nodiscard]] uint64_t GetTotalDespawned() const { return mTotalDespawned; }

    // Timing telemetry, in microseconds - isolates the actual cost of Coordinator::DestroyEntity/
    // VehicleFactory::Create from the rest of frame time, so lag can be attributed instead of guessed.
    [[nodiscard]] double GetLastFrameDestroyMicros() const { return mLastFrameDestroyUs; }
    [[nodiscard]] double GetLastFrameSpawnMicros() const { return mLastFrameSpawnUs; }
    [[nodiscard]] double GetAverageDestroyMicros() const
    {
        return mTotalDespawned > 0 ? mTotalDestroyTimeUs / static_cast<double>(mTotalDespawned) : 0.0;
    }
    [[nodiscard]] double GetAverageSpawnMicros() const
    {
        return mTotalSpawned > 0 ? mTotalSpawnTimeUs / static_cast<double>(mTotalSpawned) : 0.0;
    }
    [[nodiscard]] double GetMaxDestroyMicros() const { return mMaxDestroyTimeUs; }
    [[nodiscard]] double GetMaxSpawnMicros() const { return mMaxSpawnTimeUs; }

private:
    std::unordered_map<int, const LaneComponent*> CollectCarLanesByRow(Coordinator& coordinator) const;

    // Max vehicles a lane can actually hold on screen with a survivable gap at its own
    // top speed - clamps LaneComponent::MaxTraffics down instead of trusting it blindly.
    int GetLaneCapacity(const LaneComponent& lane) const;

    // One-time construction of the whole vehicle pool, spread across the visible road
    // instead of queued off-screen. Everything after this reuses these entities.
    void FillPool(Coordinator& coordinator);

    // Moves vehicles that crossed their lane's far edge into the shared reserve, then
    // tops each lane back up to GetLaneCapacity() from that reserve. No entity creation
    // or destruction happens here.
    void RecycleOffscreen(Coordinator& coordinator);
    void RefillLanes(Coordinator& coordinator);

    Entity ConstructVehicle(int row, const LaneComponent& lane, Coordinator& coordinator);
    void PlaceVehicleAtEntry(Entity vehicle, int row, const LaneComponent& lane, Coordinator& coordinator);

    void UpdateTelemetry(float dt);

    // Lane tiles are created one entity per grid column (see Game.cpp), so several entities
    // share the same RowIndex. Traffic is tracked per row, not per lane-tile entity.
    std::unordered_map<int, std::vector<Entity>> mLaneRuntimes;

    // Vehicles recycled from an exited lane, waiting to be reassigned to whichever lane
    // needs a refill next - shared across all lanes rather than owned by one.
    std::vector<Entity> mReserve;
    bool mPoolFilled = false;
    bool mStressTestUncapped = false;

    AssetManager* mAssets     = nullptr;
    float mWorldWidth  = 0.f;
    float mWorldHeight = 0.f;

    std::mt19937 mRng{std::random_device{}()};

    int mSpawnCountThisSecond   = 0;
    int mDespawnCountThisSecond = 0;
    int mLastSpawnsPerSecond    = 0;
    int mLastDespawnsPerSecond  = 0;
    float mTelemetryTimer       = 0.f;

    uint64_t mTotalSpawned   = 0;
    uint64_t mTotalDespawned = 0;

    double mLastFrameDestroyUs = 0.0;
    double mLastFrameSpawnUs   = 0.0;
    double mTotalDestroyTimeUs = 0.0;
    double mTotalSpawnTimeUs   = 0.0;
    double mMaxDestroyTimeUs   = 0.0;
    double mMaxSpawnTimeUs     = 0.0;
};


#endif //DUCKDUCKROAD_TRAFFICSPAWNSYSTEM_H
