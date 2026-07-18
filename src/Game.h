    //
    // Created by PC on 13-Dec-25.
    //

#ifndef DUCKDUCKROAD_GAME_H
#define DUCKDUCKROAD_GAME_H
#include "Core/AssetManager.h"
#include "Core/Coordinator.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include "Systems/AnimationSystem.h"
#include "Systems/GridMovementSystem.h"
#include "Systems/CollisionSystem.h"
#include "Systems/GameModeSystem.h"
#include "Systems/RenderSystem.h"
#include "Systems/InputSystem.h"
#include "Systems/MovementSystem.h"
#include "Systems/TrafficSpawnSystem.h"

    class Game
{
public:
    Game();
    void Run();
    void Init();
    void ProcessEvents();
    void Update(float dt);
    void Render();

private:

    unsigned int mWindowWidth = 1920u;
    unsigned int mWindowHeight = 1080u;
    uint32_t mMaxFramerate = 144;

    sf::RenderWindow mWindow;

    Entity mPlayer;
    Entity mGameRuleEntity;
    Entity mGameStateEntity;
    sf::Clock mClock;
    AssetManager mAssetManager;

    Coordinator mCoordinator;
    std::shared_ptr<GridMovementSystem> mGridMovementSystem;
    std::shared_ptr<InputSystem> mInputSystem;
    std::shared_ptr<CollisionSystem> mCollisionSystem;
    std::shared_ptr<RenderSystem> mRenderSystem;
    std::shared_ptr<GameModeSystem> mGameModeSystem;
    std::shared_ptr<MovementSystem> mMovementSystem;
    std::shared_ptr<AnimationSystem> mAnimationSystem;
    std::shared_ptr<TrafficSpawnSystem> mTrafficSpawnSystem;

    bool bDebug;
};


#endif //DUCKDUCKROAD_GAME_H