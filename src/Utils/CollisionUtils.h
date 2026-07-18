//
// Created by Hyunsoo Park on 12/26/25.
//

#ifndef COLLISIONUTILS_H
#define COLLISIONUTILS_H
#include "../Component/CollisionComponent.h"
#include "../Core/Coordinator.h"
#include "../Core/SpatialGrid.h"
#include "../Systems/CollisionSystem.h"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

struct AABBCollisionComponent;
class Coordinator;

namespace CollisionUtils
{
    inline void DebugAABB(sf::RenderWindow& inWindow, System& inSystem, Coordinator& inCoordinator)
    {
        sf::RectangleShape rect;
        rect.setFillColor(sf::Color::Transparent);
        rect.setOutlineColor(sf::Color::Red);
        rect.setOutlineThickness(1.f);

        for (auto& entity: inSystem.Entities)
        {
            auto& col = inCoordinator.GetComponent<AABBCollisionComponent>(entity);
            {
                rect.setSize(col.CollisionRect.size);
                rect.setPosition(col.CollisionRect.position);
                inWindow.draw(rect);
            }
        }

    }

    void ShowGrid(sf::RenderWindow& inWindow, const CollisionSystem& collisionSystem)
    {
        auto& grid = collisionSystem.GetGrid();
        unsigned int gridWidth = grid.GetGridWidth();
        unsigned int gridHeight = grid.GetGridHeight();
        float cellSize = grid.GetCellSize();

        for (unsigned int cy = 0; cy < gridHeight; ++ cy)
        {
            for (unsigned int cx = 0; cx < gridWidth; ++ cx)
            {
                sf::RectangleShape rect;
                rect.setFillColor(sf::Color::Transparent);
                rect.setOutlineColor(sf::Color::White);
                rect.setOutlineThickness(1.f);

                int idx = static_cast<int>(cy * gridWidth + cx);

                float screenX = cx * cellSize;
                float screenY = cy * cellSize;

                rect.setPosition(sf::Vector2f(screenX, screenY));
                rect.setSize(sf::Vector2f(grid.GetCellSize(), grid.GetCellSize()));
                if (grid.IsCellOccupied(idx))
                {
                    rect.setFillColor(sf::Color::Blue);
                }

                inWindow.draw(rect);
            }
        }
    }
}
#endif //COLLISIONUTILS_H
