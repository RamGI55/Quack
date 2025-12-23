//
// Created by PC on 18-Dec-25.
//

#ifndef DUCKDUCKROAD_SCENEMANAGER_H
#define DUCKDUCKROAD_SCENEMANAGER_H
#include <memory>
#include <string>
#include <unordered_map>

#include "IScene.h"


class IScene;

class SceneManager
{
public:

    SceneManager(Coordinator* inCoordinator) : mCoordinator(inCoordinator) { }
    template<typename T>
    void RegisterScene(const std::string& inSceneName)
    {
        std::unique_ptr<IScene> scene = std::make_unique<T>();

        scene->SetSceneManager(this);
        scene->SetCoordinator(mCoordinator);

        mScenes[inSceneName] = std::move(scene); // cannot copy the scene

    }
    void SwitchTo (const std::string& inSceneName);
    void PushOverlay (const std::string& inOverlayName);
    void PopOverlay();

    void Update(float dt);
    void Render(sf::RenderWindow& inWindow);
    void HandleEvent(const sf::Event& Event);

    inline std::string GetCurrentSceneName() const { return mCurrentSceneName; }
    inline bool HasOverlay() const { return mOverlayScene != nullptr; }


private:
    std::unordered_map<std::string, std::unique_ptr<IScene>> mScenes;
    IScene* mCurrentScene = nullptr;
    IScene* mOverlayScene = nullptr;
    std::string mCurrentSceneName;
    Coordinator* mCoordinator;


};


#endif //DUCKDUCKROAD_SCENEMANAGER_H