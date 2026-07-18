//
// Created by PC on 13-Dec-25.
//

#include "Game.h"

#include "Component/AnimationComponent.h"
#include "Component/GameStateComponent.h"
#include "Component/InputComponent.h"
#include "Component/LaneComponent.h"
#include "Component/MovementComponent.h"
#include "Component/PawnComponent.h"
#include "Component/PlayerComponent.h"
#include "Component/SoundComponent.h"
#include "Component/TransformComponent.h"
#include "Component/VelocityComponent.h"
#include "Factory/EntityFactory.h"
#include "SFML/Graphics/Text.hpp"
#include "Systems/RenderSystem.h"
#include "Utils/CollisionUtils.h"

Game::Game() :mWindow(sf::VideoMode({mWindowWidth, mWindowHeight}), "DuckDuckRoad")
{
    mWindow.setFramerateLimit(mMaxFramerate);
}

void Game::Run()
{
    Init();
    while (mWindow.isOpen()) {
        float dt = mClock.restart().asSeconds();
        ProcessEvents();
        Update(dt);
        Render();
    }
}

void Game::Init()
{
    // Initalise the coordinator

    mCoordinator.Init();

    AssetManager& assets = mAssetManager;

    mInputSystem = mCoordinator.RegisterSystem<InputSystem>();
    mGridMovementSystem = mCoordinator.RegisterSystem<GridMovementSystem>();
    mCollisionSystem = mCoordinator.RegisterSystem<CollisionSystem>();
    mRenderSystem = mCoordinator.RegisterSystem<RenderSystem>();
    mGameModeSystem = mCoordinator.RegisterSystem<GameModeSystem>();
    mMovementSystem = mCoordinator.RegisterSystem<MovementSystem>();
    mAnimationSystem = mCoordinator.RegisterSystem<AnimationSystem>();
    mTrafficSpawnSystem = mCoordinator.RegisterSystem<TrafficSpawnSystem>();

    mCoordinator.RegisterComponent<InputComponent>();
    mCoordinator.RegisterComponent<TransformComponent>();
    mCoordinator.RegisterComponent<GridMovementComponent>();
    mCoordinator.RegisterComponent<AABBCollisionComponent>();
    mCoordinator.RegisterComponent<SpriteComponent>();
    mCoordinator.RegisterComponent<GameRuleComponent>();
    mCoordinator.RegisterComponent<GameStateComponent>();
    mCoordinator.RegisterComponent<FrogRuleComponent>();
    mCoordinator.RegisterComponent<VelocityComponent>();
    mCoordinator.RegisterComponent<SoundComponent>();
    mCoordinator.RegisterComponent<AnimationComponent>();
    mCoordinator.RegisterComponent<FrogGameStateComponent>();
    mCoordinator.RegisterComponent<LaneComponent>();

    // TODO: mGameView will be part of the camera component, must be dynamic later.
    mGameView.setSize ({mGameViewWidth, mGameViewHeight});
    mGameView.setCenter({mGameViewWidth / 2.f, mGameViewHeight / 2.f});
    mGameView.setViewport(sf::FloatRect( {mMarginX / mWindowWidth, mMarginY / mWindowHeight},
{1.f - 2.f * mMarginX / mWindowWidth, 1.f - 2.f * mMarginY / mWindowHeight}));

    mCollisionSystem->Init(64.f, mGameViewWidth, mGameViewHeight);

    Signature inputSig;
    inputSig.set(mCoordinator.GetComponentType<InputComponent>());
    mCoordinator.SetSystemSignature<InputSystem>(inputSig);

    Signature sig;
    sig.set(mCoordinator.GetComponentType<TransformComponent>());
    sig.set(mCoordinator.GetComponentType<GridMovementComponent>());
    sig.set(mCoordinator.GetComponentType<InputComponent>());
    mCoordinator.SetSystemSignature<GridMovementSystem>(sig);

    Signature rendSig;
    rendSig.set(mCoordinator.GetComponentType<TransformComponent>());
    rendSig.set(mCoordinator.GetComponentType<SpriteComponent>());
    mCoordinator.SetSystemSignature<RenderSystem>(rendSig);

    Signature colSig;
    colSig.set(mCoordinator.GetComponentType<TransformComponent>());
    colSig.set(mCoordinator.GetComponentType<AABBCollisionComponent>());
    mCoordinator.SetSystemSignature<CollisionSystem>(colSig);

    Signature moveSig;
    moveSig.set(mCoordinator.GetComponentType<TransformComponent>());
    moveSig.set(mCoordinator.GetComponentType<VelocityComponent>());
    mCoordinator.SetSystemSignature<MovementSystem>(moveSig);

    Signature animSig;
    animSig.set(mCoordinator.GetComponentType<SpriteComponent>());
    animSig.set(mCoordinator.GetComponentType<AnimationComponent>());
    mCoordinator.SetSystemSignature<AnimationSystem>(animSig);

    Signature ruleSig;
    ruleSig.set(mCoordinator.GetComponentType<GameStateComponent>());
    mCoordinator.SetSystemSignature<GameModeSystem>(ruleSig);

    Signature laneSig;
    laneSig.set(mCoordinator.GetComponentType<LaneComponent>());
    mCoordinator.SetSystemSignature<TrafficSpawnSystem>(laneSig);

    EntityDef playerDef{.type = "player", .spawnX = 15, .spawnY = 15, .cellSize = 64.f};
    EntityDef CarDef {.type = "car", .spawnX = 550, .spawnY = 150, .cellSize = 50.f};

    // TODO: Hardcoded the asset loading, implements key generation algorithm in the asset loading.
    assets.Load<sf::Texture>("player_tex", "resources/Duck/duck-Sheet.png");
    assets.Load<sf::Texture>("car_tex", "resources/Coupe/coupe_blue.png");
    assets.Load<sf::Texture>("coup_green", "resources/Coupe/coupe_green.png");
    assets.Load<sf::Texture>("coup_midnight", "resources/Coupe/coupe_midnight.png");
    assets.Load<sf::Texture>("coup_red", "resources/Coupe/coupe_red.png");
    assets.Load<sf::SoundBuffer>("sound_quack", "resources/Sound/sound_quack.mp3");
    assets.Load<sf::SoundBuffer>("sound_quack2", "resources/Sound/sound_quack2.mp3");
    assets.Load<sf::SoundBuffer>("sound_dead", "resources/Sound/sound_dead.mp3");
    assets.Load<sf::SoundBuffer>("sound_gameover", "resources/Sound/sound_gameover.mp3");
    assets.Load<sf::SoundBuffer>("sound_crash", "resources/Sound/sound_crash.mp3");
    assets.Load<sf::SoundBuffer>("sound_gangnamyeoja", "resources/Sound/sound_gangnam.mp3");
    assets.Load<sf::SoundBuffer>("sound_gangnamop", "resources/Sound/sound_gangnamop.mp3");
    assets.Load<sf::SoundBuffer>("sound_gangnamahh", "resources/Sound/sound_gangnamFast.mp3");
    assets.Load<sf::SoundBuffer>("sound_train", "resources/Sound/sound_train.mp3");
    assets.Load<sf::Font>("font_main", "resources/Font/PixelifySans-Regular.ttf");

    mGameRuleEntity = mCoordinator.CreateEntity();
    mCoordinator.AddComponent(mGameRuleEntity, GameRuleComponent{.InitTime = 300.f});

    mGameStateEntity = mCoordinator.CreateEntity();
    mCoordinator.AddComponent(mGameStateEntity, FrogRuleComponent{});
    mCoordinator.AddComponent(mGameStateEntity, GameStateComponent{});
    mCoordinator.AddComponent(mGameStateEntity, FrogGameStateComponent{});

    mGameModeSystem->InitialiseGameEvents(mGameRuleEntity, mGameStateEntity, mCoordinator);

    Entity car = mCoordinator.CreateEntity();

    sf::Vector2f carPos{150, 150};
    sf::Vector2i carspawnGrid = {CarDef.spawnX, CarDef.spawnY};

    mCoordinator.AddComponent(car, TransformComponent{.Position = carPos, .Angle = sf::degrees(90.f)});
    mCoordinator.AddComponent(car, GridMovementComponent{.GridPosition = carspawnGrid});
    mCoordinator.AddComponent(car, VelocityComponent{.Velocity = {250.f, 0.0f}});

    AABBCollisionComponent col;
    col.CollisionRect.size = { 70.f, 50.f };
    col.Layer = static_cast<uint32_t>(QKCollisionType::Enemy);
    col.Mask  = static_cast<uint32_t>(QKCollisionType::Player);

    SpriteComponent carSprite;
    carSprite.setTexture(assets.Get<sf::Texture>("car_tex"));
    const sf::Vector2u carTexSize = carSprite.Texture->getSize();
    carSprite.Sprite->setOrigin({carTexSize.x / 2.f, carTexSize.y / 2.f});
    carSprite.Sprite->setScale({50.f / carTexSize.x, 50.f / carTexSize.y});
    mCoordinator.AddComponent(car, carSprite);

    mCoordinator.AddComponent(car, col);
    PlayerFactory player1(mCoordinator, assets);

    mPlayer = player1.Create(playerDef);

    // Goal strip along the top of the screen - duck reaching it scores.
    Entity goal = mCoordinator.CreateEntity();
    AABBCollisionComponent goalCol;
    goalCol.CollisionRect.size = { static_cast<float>(mWindowWidth), 64.f };
    goalCol.Layer    = static_cast<uint32_t>(QKCollisionType::Goal);
    goalCol.Mask     = static_cast<uint32_t>(QKCollisionType::Player);
    goalCol.IsStatic = false;

    mCoordinator.AddComponent(goal, TransformComponent{.Position = {mWindowWidth /2.0f , 32.f}});
    mCoordinator.AddComponent(goal, goalCol);
}

void Game::ProcessEvents()
{
    while (const std::optional event = mWindow.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            mWindow.close();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::U))
        {
            bDebug = true;
        }
    }

}

void Game::Update(float dt)
{
    mInputSystem->Update(dt, mCoordinator);
    mGridMovementSystem->Update(dt, mCoordinator);
    mMovementSystem->Update(dt, mCoordinator);
    mCollisionSystem->Update(dt, mCoordinator);
    mAnimationSystem->Update(dt, mCoordinator);
    mGameModeSystem->Update(dt, mCoordinator);

    for (const auto& event : mCollisionSystem->GetCollisionEvent())
    {
        const uint32_t playerLayer = static_cast<uint32_t>(QKCollisionType::Player);
        const uint32_t enemyLayer  = static_cast<uint32_t>(QKCollisionType::Enemy);
        const uint32_t goalLayer   = static_cast<uint32_t>(QKCollisionType::Goal);

        bool isPlayerVsEnemy = (event.LayerA == playerLayer && event.LayerB == enemyLayer)
                            || (event.LayerA == enemyLayer && event.LayerB == playerLayer);
        bool isPlayerVsGoal  = (event.LayerA == playerLayer && event.LayerB == goalLayer)
                            || (event.LayerA == goalLayer && event.LayerB == playerLayer);

        if (isPlayerVsEnemy)
        {
            Entity pawn = (event.LayerA == playerLayer) ? event.EntityA : event.EntityB;
            mGameModeSystem->LifeLostEvent(mGameStateEntity, mCoordinator);
            mGridMovementSystem->ResetToSpawn(pawn, mCoordinator);
        }
        else if (isPlayerVsGoal)
        {
            Entity pawn = (event.LayerA == playerLayer) ? event.EntityA : event.EntityB;
            mGameModeSystem->DuckSavedEvent(mGameStateEntity, mCoordinator);
            mGridMovementSystem->ResetToSpawn(pawn, mCoordinator);
        }
    }
}

void Game::Render()
{
    auto& transform = mCoordinator.GetComponent<TransformComponent>(mPlayer);
    mWindow.clear(sf::Color::Black);

    sf::RectangleShape boundary({mGameViewWidth, mGameViewHeight});
    boundary.setPosition({mMarginX, mMarginY});
    boundary.setFillColor(sf::Color::Transparent);
    boundary.setOutlineColor(sf::Color::White);
    boundary.setOutlineThickness(2.f);
    mWindow.draw(boundary);

    mWindow.setView(mGameView);
    mRenderSystem->update(mWindow, mCoordinator);

    if (bDebug == true)
    {
        CollisionUtils::DebugAABB(mWindow, *mCollisionSystem, mCoordinator);
        CollisionUtils::ShowGrid(mWindow, *mCollisionSystem);
        CollisionUtils::DebugEntityIds(mWindow, *mCollisionSystem, mCoordinator, *mAssetManager.Get<sf::Font>("font_main"));
    }

    mWindow.setView(mWindow.getDefaultView());

    auto& gameState = mCoordinator.GetComponent<GameStateComponent>(mGameStateEntity);
    auto& frogState = mCoordinator.GetComponent<FrogGameStateComponent>(mGameStateEntity);

    sf::Text hudText(*mAssetManager.Get<sf::Font>("font_main"), "DUCKS: " + std::to_string(gameState.TotalDucks), 36);
    sf::Text scoreText(*mAssetManager.Get<sf::Font>("font_main"), "SCORE: " + std::to_string(frogState.Score), 36);
    hudText.setFillColor(sf::Color::White);
    hudText.setPosition({10.f, 10.f});
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition({10.f, 50.f});

    mWindow.draw(hudText);
    mWindow.draw(scoreText);

    mWindow.display();
}

