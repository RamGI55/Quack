//
// Created by PC on 13-Dec-25.
//

#include "Game.h"

#include "Component/InputComponent.h"
#include "Component/MovementComponent.h"
#include "Component/PlayerComponent.h"
#include "Component/TransformComponent.h"

Game::Game() :mWindow(sf::VideoMode({mWindowWidth, mWindowHeight}), "DuckDuckRoad") {

    mWindow.setFramerateLimit(mMaxFramerate);
}

void Game::Run() {
    Init();
    while (mWindow.isOpen()) {
        float dt = mClock.restart().asSeconds();
        ProcessEvents();
        Update(dt);
        Render();
    }
}

void Game::Init() {

    // Initalise the coordinator
    mCoordinator.Init();

    mCoordinator.RegisterComponent<InputComponent>();
    mCoordinator.RegisterComponent<TransformComponent>();
    mCoordinator.RegisterComponent<GridMovementComponent>();

    mGridMovementSystem = mCoordinator.RegisterSystem<GridMovementSystem>();

    Signature sig;
    sig.set(mCoordinator.GetComponentType<TransformComponent>());
    sig.set(mCoordinator.GetComponentType<GridMovementComponent>());
    sig.set(mCoordinator.GetComponentType<InputComponent>());
    mCoordinator.SetSystemSignature<GridMovementSystem>(sig);

    mPlayer = mCoordinator.CreateEntity();

    sf::Vector2i startGrid = { 15, 15};
    float cellSize = 64.f;
    sf::Vector2f startPos ={
    startGrid.x * cellSize + cellSize / 2,
    startGrid.y * cellSize + cellSize / 2
    };

    mCoordinator.AddComponent(mPlayer, TransformComponent{.Position = startPos});
    mCoordinator.AddComponent(mPlayer, GridMovementComponent{.GridPosition = startGrid});
    mCoordinator.AddComponent(mPlayer, InputComponent{});

    // Temp visual
    mTempRectangle.setSize({cellSize - 4.f, cellSize - 4.f});
    mTempRectangle.setFillColor(sf::Color::Green);
    mTempRectangle.setOrigin({(cellSize - 4.f) / 2.f, (cellSize - 4.f) / 2.f});

}

void Game::ProcessEvents() {

    while (const std::optional event = mWindow.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            mWindow.close();
        }
    }

}

void Game::Update(float dt) {

    mGridMovementSystem->Update(dt, mCoordinator);
}

void Game::Render() {
    auto& transform = mCoordinator.GetComponent<TransformComponent>(mPlayer);
     mTempRectangle.setPosition(transform.Position);  // center of screen

    mWindow.clear(sf::Color::Black);
    mWindow.draw(mTempRectangle);
    mWindow.display();
}

