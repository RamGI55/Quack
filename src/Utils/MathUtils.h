//
// Created by Hyunsoo Park on 12/25/25.
//

#ifndef MATHUTILS_H
#define MATHUTILS_H
#include <random>

#include "SFML/System/Vector2.hpp"

namespace MathUtils
{
    inline sf::Vector2f Lerp(const sf::Vector2f& a, const sf::Vector2f& b, float t)
    {
        return a + (b - a) * t;
    }
    inline sf::Vector2f GridToWorld(const sf::Vector2i gridPos, const float gridSize)
    {
        return {gridPos.x * gridSize + gridSize /2.f ,gridPos.y * gridSize + gridSize /2.f };
    }

    inline int GenerateRandomInt(int min, int max)
    {
        std::random_device rd;
        std::mt19937 rng(rd());
        std::uniform_int_distribution<std::mt19937::result_type> distribution(min, max);
        return (int)distribution(rng);
    }
}

#endif //MATHUTILS_H
