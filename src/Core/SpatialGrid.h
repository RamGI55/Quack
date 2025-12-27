//
// Created by Hyunsoo Park on 12/26/25.
//

#ifndef DUCKDUCKROAD_SPATIALGRID_H
#define DUCKDUCKROAD_SPATIALGRID_H
#include <unordered_map>
#include "Types.h"
#include <vector>
#include <set>
#include <algorithm>
#include "SFML/Graphics/Rect.hpp"


class SpatialGrid {

public:
    SpatialGrid(float CellSize, unsigned int WorldWidth, unsigned int WorldHeight);
    void Insert(Entity InEntity, const sf::FloatRect& BoundingBox);
    void Remove(Entity InEntity);
    void Update(Entity InEntity, const sf::FloatRect& BoundingBox);
    std::vector<Entity> GetNearby(const sf::FloatRect& BoundingBox) const;
    std::vector<int> GetOverlappingCells(const sf::FloatRect& BoundingBox) const;
    void Clear();



private:
    int PositionToIndex (float x, float y) const;
    bool IsValidIndex(int Index) const;
    float mCellSize = 64.f;
    unsigned int mGridWidth;
    unsigned int mGridHeight;
    std::vector<std::vector<Entity>> mCells;
    std::unordered_map<Entity, int> mEntityToCell;
};


#endif //DUCKDUCKROAD_SPATIALGRID_H