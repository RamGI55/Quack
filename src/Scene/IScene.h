//
// Created by PC on 24-Dec-25.
//

#ifndef DUCKDUCKROAD_ISCENE_H
#define DUCKDUCKROAD_ISCENE_H
#include "SceneManager.h"
#include "SFML/Graphics/RenderWindow.hpp"

class SceneManager;
class Coordinator;
class IScene;

class IScene
{
public:
    ~IScene() = default;

    virtual void OnEnter()  = 0;
    virtual void OnExit()   = 0;
    virtual void Update(float dt) = 0;
    virtual void Render(sf::RenderWindow& window) = 0;
    virtual void HandleEvent(const sf::Event& Event) = 0;

    void SetSceneManager(SceneManager* inSceneManager) { m_SceneManager = inSceneManager; }
    void SetCoordinator(Coordinator* inCoordinator) {m_Coordinator = inCoordinator;}

protected:
    SceneManager* m_SceneManager = nullptr;
    Coordinator* m_Coordinator = nullptr;


};
#endif //DUCKDUCKROAD_ISCENE_H