//
// Created by PC on 18-Dec-25.
//

#include "../Scene/SceneManager.h"

void SceneManager::SwitchTo(const std::string& inSceneName)
{
   if (mCurrentScene != nullptr)
   {
      mCurrentScene->OnExit();
   }
   if (mScenes.find(inSceneName) != mScenes.end())
   {
      mCurrentScene = mScenes[inSceneName].get();
      mCurrentSceneName = inSceneName;
      mCurrentScene->OnEnter();
   }
}

void SceneManager::PushOverlay(const std::string& inOverlayName)
{
   if (mScenes.find(inOverlayName) != mScenes.end())
   {
      mOverlayScene = mScenes[inOverlayName].get();
      mOverlayScene->OnEnter();
   }

   // DO NOT TOUCH mCurrentScene! both overlady and current scene must be played!
}

void SceneManager::PopOverlay()
{
   if (mOverlayScene != nullptr)
   {
      mOverlayScene->OnExit();
      mOverlayScene = nullptr;
   }
}

void SceneManager::Update(float dt)
{
   if (mOverlayScene != nullptr)
   {
      mOverlayScene->Update(dt);
   }
   else if (mCurrentScene != nullptr)
   {
      mCurrentScene->Update(dt);
   }
}

void SceneManager::Render(sf::RenderWindow& inWindow)
{
   if (mCurrentScene != nullptr)
   {
      mCurrentScene->Render(inWindow);
   }
   // must render overlay scene and current scene both!
   if (mOverlayScene != nullptr)
   {
      mOverlayScene->Render(inWindow);
   }
}

void SceneManager::HandleEvent(const sf::Event& Event)
{
   if (mOverlayScene != nullptr)
   {
      mOverlayScene->HandleEvent(Event);
   }
   else if (mCurrentScene != nullptr)
   {
      mCurrentScene->HandleEvent(Event);
   }
}
