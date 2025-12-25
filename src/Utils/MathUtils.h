//
// Created by Hyunsoo Park on 12/25/25.
//

#ifndef MATHUTILS_H
#define MATHUTILS_H
#include "SFML/System/Vector2.hpp"

namespace MathUtils {
    inline sf::Vector2f Lerp(const sf::Vector2f& a, const sf::Vector2f& b, float t)
    {
        return a + (b - a) * t;
    }
    inline sf::Vector2f GridToWorld(const sf::Vector2i gridPos, const float gridSize)
    {
        return {
            gridPos.x * gridSize + gridSize /2.f ,
            gridPos.y * gridSize + gridSize /2.f };
    }

}

#endif //MATHUTILS_H
