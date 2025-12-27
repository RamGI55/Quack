/*
    1. Update bounding boxes from transforms
    2. Update entity positions in SpatialGrid
    3. Check for collisions using GetNearby
    4. Store collision events for other systems to read
 */

#ifndef DUCKDUCKROAD_COLLISIONSYSTEM_H
#define DUCKDUCKROAD_COLLISIONSYSTEM_H
#include "../Core/Coordinator.h"
#include "../Core/Types.h"
#include "../Core/SpatialGrid.h"

struct CollisionEvent
{
    Entity EntityA;
    Entity EntityB;
    uint32_t LayerA;
    uint32_t LayerB;
};

class CollisionSystem : public System
{
public:
    CollisionSystem(SystemManager* manager = nullptr);
    void Init(float CellSize, unsigned int WorldWidth, unsigned int WorldHeight);
    void Update(float dt, Coordinator& coordinator);
    const std::vector<CollisionEvent>& GetCollisionEvent() const;
    void ClearEvents();

private:
    bool CheckAABB(const sf::FloatRect& a, const sf::FloatRect& b);
    bool ShouldCollide(uint32_t layerA, uint32_t maskA, uint32_t layerB, uint32_t maskB);

private:
    std::unique_ptr<SpatialGrid> mGrid = nullptr;
    std::vector<CollisionEvent> mCollisionEvents;

};


#endif //DUCKDUCKROAD_COLLISIONSYSTEM_H