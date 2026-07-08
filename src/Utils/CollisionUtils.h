//
// Created by Hyunsoo Park on 12/26/25.
//

#ifndef COLLISIONUTILS_H
#define COLLISIONUTILS_H
#include "../Component/CollisionComponent.h"
#include "../Core/Coordinator.h"
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
}
#endif //COLLISIONUTILS_H
