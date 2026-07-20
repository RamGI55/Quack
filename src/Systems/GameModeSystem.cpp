//
// Created by PC on 09-Jul-26.
//

#include "GameModeSystem.h"

#include "../Component/GameStateComponent.h"
#include "../Component/PawnComponent.h"
#include "../Core/Coordinator.h"

struct GameStateComponent;

GameModeSystem::GameModeSystem(SystemManager* manager)
{
}

GameModeSystem::~GameModeSystem()
{
}

void GameModeSystem::init(Coordinator& coordinator)
{

}

void GameModeSystem::Update(float dt, Coordinator& coordinator)
{
    for (auto& entity : Entities)
    {
        auto& gameState = coordinator.GetComponent<GameStateComponent>(entity);
        if (gameState.state == GameState::InGame)
        {
            gameState.CurrentTime -= dt;
        }

        if (gameState.CurrentTime <= 0.0f)
            gameState.state = GameState::GameOver;

    }
}

void GameModeSystem::ResetGameEvent(Entity inGamestateEntity, Coordinator& coordinator)
{

}


void GameModeSystem::LifeLostEvent(Entity inGamestateEntity, Coordinator& coordinator)
{
    auto& gamestate = coordinator.GetComponent<GameStateComponent>(inGamestateEntity);

    --gamestate.TotalDucks;
    if (gamestate.TotalDucks == 0)
    {
        GameOverEvent(inGamestateEntity, coordinator);
    }
}

void GameModeSystem::DuckSavedEvent(Entity inGamestateEntity, Coordinator& coordinator)
{
    auto& frogRule  = coordinator.GetComponent<FrogRuleComponent>(inGamestateEntity);
    auto& gamestate = coordinator.GetComponent<GameStateComponent>(inGamestateEntity);
    auto& frogState = coordinator.GetComponent<FrogGameStateComponent>(inGamestateEntity);

    ++gamestate.FilledDucks;
    frogState.Score += frogRule.SuccessEventPoints;
}

void GameModeSystem::GameOverEvent(Entity inGamestateEntity, Coordinator& coordinator)
{
    auto& gamestate = coordinator.GetComponent<GameStateComponent>(inGamestateEntity);

    gamestate.state = GameState::GameOver;
}

void GameModeSystem::ClearEvent(Coordinator& coordinator)
{

}

void GameModeSystem::InitialiseGameEvents(Entity inGameruleEntity, Entity inGamestateEntity, Coordinator& coordinator)
{
    auto& gameRule = coordinator.GetComponent<GameRuleComponent>(inGameruleEntity);
    auto& frogRule = coordinator.GetComponent<FrogRuleComponent>(inGamestateEntity);
    auto& gamestate = coordinator.GetComponent<GameStateComponent>(inGamestateEntity);

    gamestate.state = GameState::InGame;
    gamestate.CurrentTime = gameRule.InitTime;
    gamestate.TotalDucks = frogRule.InitLives;
    gamestate.FilledDucks = 0;
}


