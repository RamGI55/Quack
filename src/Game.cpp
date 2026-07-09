//
// Created by PC on 13-Dec-25.
//

#include "Game.h"

#include "Component/InputComponent.h"
#include "Component/MovementComponent.h"
#include "Component/PlayerComponent.h"
#include "Component/TransformComponent.h"
#include "Factory/EntityFactory.h"
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

    AssetManager assets;

    mCoordinator.RegisterComponent<InputComponent>();
    mCoordinator.RegisterComponent<TransformComponent>();
    mCoordinator.RegisterComponent<GridMovementComponent>();
    mCoordinator.RegisterComponent<AABBCollisionComponent>();
    mCoordinator.RegisterComponent<SpriteComponent>();

    mInputSystem = mCoordinator.RegisterSystem<InputSystem>();
    mGridMovementSystem = mCoordinator.RegisterSystem<GridMovementSystem>();
    mCollisionSystem = mCoordinator.RegisterSystem<CollisionSystem>();
    mRenderSystem = mCoordinator.RegisterSystem<RenderSystem>();

    mCollisionSystem->Init(64.f,mWindowHeight, mWindowWidth);

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

    EntityDef playerDef{.type = "player", .spawnX = 15, .spawnY = 15, .cellSize = 125.f};

    PlayerFactory player1(mCoordinator, assets);
    mPlayer = player1.Create(playerDef);

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
    mCollisionSystem->Update(dt, mCoordinator);

}

void Game::Render()
{
    auto& transform = mCoordinator.GetComponent<TransformComponent>(mPlayer);

    mWindow.clear(sf::Color::Black);
    mRenderSystem->update(mWindow, mCoordinator);
    if (bDebug == true)
    {
        CollisionUtils::DebugAABB(mWindow, *mCollisionSystem, mCoordinator);
    }
    mWindow.display();
}

