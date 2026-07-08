//
// Created by Hyunsoo Park on 12/27/25.
//

#ifndef DUCKDUCKROAD_ENTITYFACTORY_H
#define DUCKDUCKROAD_ENTITYFACTORY_H
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
#include <iostream> // DEBUG: component lifetime probe, remove later



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

class PlayerFactory : EntityFactory
{
public:
    PlayerFactory(Coordinator& inCoordinator, AssetManager& inAssetManager):EntityFactory(inCoordinator, inAssetManager){}
    virtual Entity Create(EntityDef& inEntityDef) override
    {
        Entity player = mCoordinator.CreateEntity();

        sf::Vector2i spawnGrid = {inEntityDef.spawnX, inEntityDef.spawnY};
        float cellSize = inEntityDef.cellSize;
        sf::Vector2f startPos = {
            spawnGrid.x * cellSize + cellSize / 2,
            spawnGrid.y * cellSize + cellSize / 2
        };

        AABBCollisionComponent playerCol;
        playerCol.CollisionRect.size = { cellSize, cellSize };
        playerCol.Layer = static_cast<uint32_t>(QKCollisionType::Player);
        playerCol.Mask  = static_cast<uint32_t>(QKCollisionType::Enemy | QKCollisionType::Blocked | QKCollisionType::Terrain);
        playerCol.Visualised = true;

        SpriteComponent spriteCom;
        spriteCom.setTexture(mAssetManager.Get<sf::Texture>("player_tex")); // TODO: change this player texture, currently it shows fall back.
        spriteCom.Sprite->setScale({ cellSize / 16.f, cellSize / 16.f });
        mCoordinator.AddComponent(player, spriteCom);

        mCoordinator.AddComponent(player, TransformComponent{.Position = startPos});
        mCoordinator.AddComponent(player, GridMovementComponent{.GridPosition = spawnGrid});
        mCoordinator.AddComponent(player, InputComponent{});
        mCoordinator.AddComponent(player, playerCol);

        return player;
    }


private:
    float cellSize = 64.f;
    sf::Vector2f spawnPos;

};

class ObstructFactory : EntityFactory
{
public:
    ObstructFactory(Coordinator& inCoordinator, AssetManager& inAssetManager):EntityFactory(inCoordinator, inAssetManager){}
    virtual Entity Create(EntityDef& inEntityDef) override
    {
        Entity obs = mCoordinator.CreateEntity();
        sf::Vector2i spawnGrid = {inEntityDef.spawnX, inEntityDef.spawnY};
        float cellSize = inEntityDef.cellSize;
        sf::Vector2f startPos = {
            spawnGrid.x * cellSize + cellSize / 2,
            spawnGrid.y * cellSize + cellSize / 2
        };

        AABBCollisionComponent obsCol;
        obsCol.CollisionRect.size = { cellSize, cellSize };
        obsCol.Layer = static_cast<uint32_t>(QKCollisionType::Blocked);
        obsCol.Mask  = static_cast<uint32_t>(QKCollisionType::Enemy | QKCollisionType::Player | QKCollisionType::Terrain);
        obsCol.Visualised = true;

        SpriteComponent spriteCom;
        spriteCom.setTexture(mAssetManager.Get<sf::Texture>("player_tex")); // TODO: change this player texture, currently it shows fall back.
        spriteCom.Sprite->setScale({ cellSize / 16.f, cellSize / 16.f });
        mCoordinator.AddComponent(obs, spriteCom);

        mCoordinator.AddComponent(obs, TransformComponent{.Position = startPos});
        mCoordinator.AddComponent(obs, GridMovementComponent{.GridPosition = spawnGrid});
        mCoordinator.AddComponent(obs, InputComponent{});
        mCoordinator.AddComponent(obs, obsCol);

        return obs;
    }

};

class EnemyFactory : EntityFactory
{
    EnemyFactory(Coordinator& inCoordinator, AssetManager& inAssetManager):EntityFactory(inCoordinator, inAssetManager){}
    virtual Entity Create(EntityDef& inEntityDef) override
    {
        Entity enemy = mCoordinator.CreateEntity();
        sf::Vector2i spawnGrid = {inEntityDef.spawnX, inEntityDef.spawnY};
        float cellSize = inEntityDef.cellSize;
        sf::Vector2f startPos = {
            spawnGrid.x * cellSize + cellSize / 2,
            spawnGrid.y * cellSize + cellSize / 2
        };

        AABBCollisionComponent enemyCol;
        enemyCol.CollisionRect.size = { cellSize, cellSize };
        enemyCol.Layer = static_cast<uint32_t>(QKCollisionType::Enemy);
        enemyCol.Mask  = static_cast<uint32_t>(QKCollisionType::Blocked | QKCollisionType::Player | QKCollisionType::Terrain);
        enemyCol.Visualised = true;

        SpriteComponent spriteCom;
        spriteCom.setTexture(mAssetManager.Get<sf::Texture>("player_tex")); // TODO: change this player texture, currently it shows fall back.
        spriteCom.Sprite->setScale({ cellSize / 16.f, cellSize / 16.f });
        mCoordinator.AddComponent(enemy, spriteCom);

        mCoordinator.AddComponent(enemy, TransformComponent{.Position = startPos});
        mCoordinator.AddComponent(enemy, GridMovementComponent{.GridPosition = spawnGrid});
        mCoordinator.AddComponent(enemy, InputComponent{});
        mCoordinator.AddComponent(enemy, enemyCol);

        return enemy;

    }
};


#endif //DUCKDUCKROAD_ENTITYFACTORY_H