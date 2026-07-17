//
// Created by Hyunsoo Park on 12/27/25.
//

#ifndef DUCKDUCKROAD_ENTITYFACTORY_H
#define DUCKDUCKROAD_ENTITYFACTORY_H
#include "../Component/AnimationComponent.h"
#include "../Component/CollisionComponent.h"
#include "../Component/InputComponent.h"
#include "../Component/MovementComponent.h"
#include "../Component/SpritesComponent.h"
#include "../Core/Coordinator.h"
#include "../Core/Types.h"
#include "SFML/System/Vector2.hpp"
#include "../Component/TransformComponent.h"
#include "../Games/Quack/QKCollisionLayer.h"
#include "../Core/AssetManager.h"


class Coordinator;

struct EntityDef
{
    std::string type;
    int spawnX;
    int spawnY;
    float cellSize;
    sf::Texture* texture;

};

class EntityFactory
{
public:
    EntityFactory(Coordinator& inCoordinator, AssetManager& inAssetManager):mCoordinator(inCoordinator), mAssetManager(inAssetManager)
    {}
    virtual ~EntityFactory() = default;
    virtual Entity Create(EntityDef& inEntityDef) = 0;


protected:
    Coordinator& mCoordinator;
    AssetManager& mAssetManager;
};

// TODO: Make it archtype table or other alternative generation logics.
class PlayerFactory : EntityFactory
{
public:
    PlayerFactory(Coordinator& inCoordinator, AssetManager& inAssetManager):EntityFactory(inCoordinator, inAssetManager){}
    virtual Entity Create(EntityDef& inEntityDef) override
    {
        Entity player = mCoordinator.CreateEntity();

        sf::Vector2i spawnGrid = {inEntityDef.spawnX, inEntityDef.spawnY};
        cellSize = inEntityDef.cellSize;
        sf::Vector2f startPos = {
            spawnGrid.x * cellSize + cellSize / 2,
            spawnGrid.y * cellSize + cellSize / 2
        };

        AABBCollisionComponent playerCol;
        playerCol.CollisionRect.size = { cellSize, cellSize };
        playerCol.Layer = static_cast<uint32_t>(QKCollisionType::Player);
        playerCol.Mask  = static_cast<uint32_t>(QKCollisionType::Enemy | QKCollisionType::Blocked | QKCollisionType::Terrain | QKCollisionType::Goal);
        playerCol.Visualised = true;

        SpriteComponent spriteCom;
        spriteCom.setTexture(mAssetManager.Get<sf::Texture>("player_tex"));
        mCoordinator.AddComponent(player, spriteCom);

        // Row 2 = front-facing idle pose (see GridMovementSystem.cpp for the full row layout).
        AnimationComponent animCom;
        animCom.FrameSize = {32, 32};
        animCom.Row        = 2;
        animCom.FrameCount = 8;
        mCoordinator.AddComponent(player, animCom);

        // RenderSystem drives sf::Sprite scale from TransformComponent::Scale every frame,
        // so the sprite's own scale must be set here rather than on the SpriteComponent's sf::Sprite directly.
        mCoordinator.AddComponent(player, TransformComponent{.Position = startPos, .Scale = {cellSize / 32.f, cellSize / 32.f}});
        mCoordinator.AddComponent(player, GridMovementComponent{.GridPosition = spawnGrid, .SpawnGridPosition = spawnGrid});
        mCoordinator.AddComponent(player, InputComponent{});
        mCoordinator.AddComponent(player, playerCol);

        return player;
    }


private:
    // TODO: Delete it, factory cannot have state.
    float cellSize = 64.f;
    sf::Vector2f spawnPos;

};

#endif //DUCKDUCKROAD_ENTITYFACTORY_H