//
// Created by Hyunsoo Park on 12/26/25.
//

#ifndef DUCKDUCKROAD_SPATIALGRID_H
#define DUCKDUCKROAD_SPATIALGRID_H
#include <unordered_map>
#include "Types.h"
#include <vector>
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

    // Debug/render accessors - expose grid layout so it can be drawn on screen.
    float GetCellSize() const { return mCellSize; }
    unsigned int GetGridWidth() const { return mGridWidth; }
    unsigned int GetGridHeight() const { return mGridHeight; }
    bool IsCellOccupied(int Index) const { return IsValidIndex(Index) && !mCells[Index].empty(); }



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