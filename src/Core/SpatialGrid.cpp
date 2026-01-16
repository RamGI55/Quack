#include "SpatialGrid.h"
#include <iostream>

SpatialGrid::SpatialGrid(float CellSize, unsigned int WorldWidth, unsigned int WorldHeight)
    : mCellSize(CellSize)
{
    mGridWidth = static_cast<int>(WorldWidth / CellSize);
    mGridHeight = static_cast<int>(WorldHeight / CellSize);
    mCells.resize(mGridWidth * mGridHeight);
}

int SpatialGrid::PositionToIndex(float x, float y) const
{
    // both coordinates and cellsize are float - must cast to the integer to make the grid.
    int cellX = static_cast<int>(x / mCellSize);
    int cellY = static_cast<int>(y / mCellSize);

    return cellY * mGridWidth + cellX;
}

bool SpatialGrid::IsValidIndex(int index) const
{
    return index >= 0 && index < static_cast<int>(mCells.size());
}

void SpatialGrid::Insert(Entity InEntity, const sf::FloatRect& BoundingBox)
{
    sf::Vector2f centre = BoundingBox.getCenter();
    int idx = PositionToIndex(centre.x, centre.y);
    
    if (!IsValidIndex(idx)) return;
    
    mCells[idx].push_back(InEntity);
    mEntityToCell[InEntity] = idx;
}

void SpatialGrid::Remove(Entity InEntity)
{
    auto it = mEntityToCell.find(InEntity);
    if (it == mEntityToCell.end()) return;
    
    int cellIdx = it->second;
    auto& cell = mCells[cellIdx];
    
    auto findIt = std::find(cell.begin(), cell.end(), InEntity);
    if (findIt != cell.end())
    {
        cell.erase(findIt);
    }
    mEntityToCell.erase(it);
}

void SpatialGrid::Update(Entity InEntity, const sf::FloatRect& BoundingBox)
{
    sf::Vector2f centre = BoundingBox.getCenter();
    int newIdx = PositionToIndex(centre.x, centre.y);
    
    if (!IsValidIndex(newIdx)) return;
    
    auto it = mEntityToCell.find(InEntity);
    if (it == mEntityToCell.end())
    {
        // Not in grid yet, insert
        mCells[newIdx].push_back(InEntity);
        mEntityToCell[InEntity] = newIdx;
        return;
    }
    
    int oldIdx = it->second;
    if (oldIdx == newIdx) return;  // Same cell, no change
    
    // Move to new cell
    auto& oldCell = mCells[oldIdx];
    auto findIt = std::find(oldCell.begin(), oldCell.end(), InEntity);
    if (findIt != oldCell.end())
    {
        oldCell.erase(findIt);
    }
    
    mCells[newIdx].push_back(InEntity);
    mEntityToCell[InEntity] = newIdx;
}

std::vector<Entity> SpatialGrid::GetNearby(const sf::FloatRect& BoundingBox) const
{
    std::vector<Entity> result;
    
    // Get cell from center of bounding box
    sf::Vector2f centre = BoundingBox.getCenter();
    int centerCellX = static_cast<int>(centre.x / mCellSize);
    int centerCellY = static_cast<int>(centre.y / mCellSize);
    
    // Check 3x3 area around center cell
    for (int dy = -1; dy <= 1; dy++)
    {
        for (int dx = -1; dx <= 1; dx++)
        {
            int checkX = centerCellX + dx;
            int checkY = centerCellY + dy;
            
            // Bounds check
            if (checkX < 0 || checkX >= mGridWidth) continue;
            if (checkY < 0 || checkY >= mGridHeight) continue;
            
            int cellIdx = checkY * mGridWidth + checkX;
            
            // Add all entities in this cell
            for (Entity e : mCells[cellIdx])
            {
                result.push_back(e);
            }
        }
    }
    
    return result;
}

void SpatialGrid::Clear()
{
    for (auto& cell : mCells)
    {
        cell.clear();
    }
    mEntityToCell.clear();
}