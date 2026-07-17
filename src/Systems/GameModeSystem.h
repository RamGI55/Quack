//
// Created by PC on 09-Jul-26.
//

#ifndef DUCKDUCKROAD_GAMEMODESYSTEM_H
#define DUCKDUCKROAD_GAMEMODESYSTEM_H
#include "../Component/GameRuleComponent.h"
#include "../Core/System.h"

class Coordinator;
class SystemManager;

class GameModeSystem : public System
{
public:
    GameModeSystem(SystemManager* manager);
    ~GameModeSystem() override;
    void init(Coordinator& coordinator);
    void Update(float dt, Coordinator& coordinator);

    void ResetGameEvent(Entity inGamestateEntity, Coordinator& coordinator);

    void LifeLostEvent(Entity inGamestateEntity, Coordinator& coordinator);
    void DuckSavedEvent(Entity inGamestateEntity, Coordinator& coordinator);
    void GameOverEvent(Entity inGamestateEntity, Coordinator& coordinator);
    void ClearEvent(Coordinator& coordinator);
    void InitialiseGameEvents(Entity inGameruleEntity, Entity inGamestateEntity, Coordinator& coordinator);


private:


};

#endif //DUCKDUCKROAD_GAMEMODESYSTEM_H