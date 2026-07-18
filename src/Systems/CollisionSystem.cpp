//
// Created by Hyunsoo Park on 12/26/25.
//

#include "CollisionSystem.h"

#include <iostream>
#include <unordered_set>

#include "../Component/CollisionComponent.h"
#include "../Component/TransformComponent.h"

CollisionSystem::CollisionSystem(SystemManager* manager)
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
        AABBCollision.CollisionRect.position =
        {
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

        // TEMP DEBUG: brute-force ground truth (no grid involved), diffed against the
        // grid's candidate list. Any entity that truly overlaps but is missing from
        // `nearby` proves the grid - not layer/mask logic or CheckAABB - dropped it.
        {
            std::unordered_set<Entity> nearbySet(nearby.begin(), nearby.end());
            for (auto entityB : Entities)
            {
                if (entityB == entityA) continue;
                auto& bruteCollisionB = coordinator.GetComponent<AABBCollisionComponent>(entityB);
                auto overlap = collisionA.CollisionRect.findIntersection(bruteCollisionB.CollisionRect);
                if (overlap.has_value() && nearbySet.find(entityB) == nearbySet.end())
                {
                    const auto& boxA = collisionA.CollisionRect;
                    const auto& boxB = bruteCollisionB.CollisionRect;
                    std::cout << "[AABB HIT / GRID MISS] "
                              << "entityA=" << entityA
                              << " boxA=(" << boxA.position.x << "," << boxA.position.y
                              << " " << boxA.size.x << "x" << boxA.size.y << ")"
                              << " entityB=" << entityB
                              << " boxB=(" << boxB.position.x << "," << boxB.position.y
                              << " " << boxB.size.x << "x" << boxB.size.y << ")"
                              << " overlap=(" << overlap->position.x << "," << overlap->position.y
                              << " " << overlap->size.x << "x" << overlap->size.y << ")\n";
                }
            }
        }

        for (auto entityB : nearby)
        {
            if (entityA == entityB)
            {
                continue;
            }

            if (entityA >= entityB)
            {
                continue;
            }

            auto& collisionB = coordinator.GetComponent<AABBCollisionComponent>(entityB);

            bool shouldCollide = ShouldCollide(collisionA.Layer, collisionA.Mask,collisionB.Layer, collisionB.Mask);
            bool aabbHit = CheckAABB(collisionA.CollisionRect, collisionB.CollisionRect);

            if (shouldCollide && aabbHit)
            {
                mCollisionEvents.push_back
                ({
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


