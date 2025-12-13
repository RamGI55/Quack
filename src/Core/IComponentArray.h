/*
 *  IComponentArray
 *  The simple continous array to grab the relevent component for an array
 *  - The Array must be "packed", has no holes.
 *  - The iteraton must be done without valid check; All of the element in the array must be valid.
 */

#ifndef DUCKDUCKROAD_ICOMPONENTARRAY_H
#define DUCKDUCKROAD_ICOMPONENTARRAY_H
#include "Types.h"
#include <array>
#include <unordered_map>

class IComponentArray
{
public:
    virtual ~IComponentArray() = default;
    virtual void OnEntityDestroyed(Entity entity) = 0;
};

template <typename T>
class ComponentArray : public IComponentArray
{
public:
    void InsertData(Entity inEntity, T inComponent)
    {
        size_t NewIndex = Size_;
        EntityToIndex_[inEntity] = NewIndex;
        IndexToEntity_[NewIndex] = inEntity;
        ComponentArray_[NewIndex] = inComponent;
        ++Size_;

    }

    void RemoveData(Entity inEntity)
    {
        // find the entity index to remove
        size_t IndexForRemovedEntity = EntityToIndex_[inEntity];
        size_t NewIndex = Size_ - 1; // get the last index of the array
        ComponentArray_[NewIndex] = ComponentArray_[IndexForRemovedEntity];

        // Update map to point to move the spot
        Entity EntityToRemove = IndexToEntity_[IndexForRemovedEntity];
        EntityToIndex_[EntityToRemove] = IndexForRemovedEntity;
        IndexToEntity_[IndexForRemovedEntity] = EntityToRemove;

        EntityToIndex_.erase(EntityToRemove);
        IndexToEntity_.erase(IndexForRemovedEntity);

        --Size_;
    }
    T& GetData(Entity inEntity)
    {
        return ComponentArray_[EntityToIndex_[inEntity]];
    }

    void OnEntityDestroyed(Entity inEntity) override
    {
        if (EntityToIndex_.find(inEntity) != EntityToIndex_.end())
        {
            RemoveData(inEntity);
        }
    }


private:
    std::array<T, MAX_ENTITIES> ComponentArray_;
    std::unordered_map<Entity, size_t> EntityToIndex_;
    std::unordered_map<size_t, Entity> IndexToEntity_;

    // Total size of valid entries in the array.
    size_t Size_ = 0;
};

#endif //DUCKDUCKROAD_ICOMPONENTARRAY_H