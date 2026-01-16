//
// Created by Hyunsoo Park on 12/26/25.
//

#include "CollisionSystem.h"

#include "../Component/CollisionComponent.h"
#include "../Component/TransformComponent.h"

CollisionSystem::CollisionSystem(SystemManager* manager )
{
}

void CollisionSystem::Init(float CellSize, unsigned int WorldWidth, unsigned int WorldHeight)
{
    mGrid = std::make_unique<SpatialGrid>(CellSize, WorldWidth, WorldHeight) ;
}

void CollisionSystem::Update(float dt, Coordinator &coordinator)
{
    // Clear the event before its update
    ClearEvents();

    for (auto& entity : Entities)
    {
        auto& Transform = coordinator.GetComponent<TransformComponent>(entity);
        auto& AABBCollision = coordinator.GetComponent<AABBCollisionComponent>(entity);


        // Convert centre position to the top-left
        // SFML standards detect the collision by the top-left.
        AABBCollision.CollisionRect.position = {
            Transform.Position.x - AABBCollision.CollisionRect.size.x / 2.f,
            Transform.Position.y - AABBCollision.CollisionRect.size.y / 2.f
        };


        mGrid->Update(entity, AABBCollision.CollisionRect);

    }


    // collision checking loop O(n^2) to O(n * m) - checks the nearby entities.
    for (auto entityA : Entities)
    {
        auto& collisionA = coordinator.GetComponent<AABBCollisionComponent>(entityA);
        auto nearby = mGrid->GetNearby(collisionA.CollisionRect);

        for (auto entityB : nearby)
        {

            if (entityA == entityB) {
                continue;
            }

            if (entityA >= entityB)
            {

                continue;
            }

            auto& collisionB = coordinator.GetComponent<AABBCollisionComponent>(entityB);

            bool shouldCollide = ShouldCollide(collisionA.Layer, collisionA.Mask,
            collisionB.Layer, collisionB.Mask);
            bool aabbHit = CheckAABB(collisionA.CollisionRect, collisionB.CollisionRect);


            if (shouldCollide && aabbHit)
            {
                mCollisionEvents.push_back({
                    entityA,
                    entityB,
                    collisionA.Layer,
                    collisionB.Layer
                });
            }
        }
    }


}

const std::vector<CollisionEvent> & CollisionSystem::GetCollisionEvent() const
{
    return mCollisionEvents;
}

void CollisionSystem::ClearEvents()
{
    mCollisionEvents.clear();
}

bool CollisionSystem::CheckAABB(const sf::FloatRect &a, const sf::FloatRect &b)
{
    return a.findIntersection(b).has_value();
}

bool CollisionSystem::ShouldCollide(uint32_t layerA, uint32_t maskA, uint32_t layerB, uint32_t maskB)
{
    bool aWantsB = (layerA & maskB) != 0;
    bool bWantsA = (layerB & maskA) != 0;
    return aWantsB && bWantsA;
}


