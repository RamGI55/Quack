/*
 *  IComponentArray
 *  The simple continous array to grab the relevent component for an array
 *  - The Array must be "packed", no holes.
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
        // assert for the corruptions.w
        assert(m_EntityToIndex.find(inEntity) == m_EntityToIndex.end()&& "Component already exists for this entity!");

        size_t NewIndex = m_Size;
        m_EntityToIndex[inEntity] = NewIndex;
        m_IndexToEntity[NewIndex] = inEntity;
        m_ComponentArray[NewIndex] = inComponent;
        ++m_Size;

    }

    void RemoveData(Entity inEntity)
    {
        assert(m_EntityToIndex.find(inEntity) != m_EntityToIndex.end() && "Tried to remove an entity that does not exist!");

        // find the entity index to remove
        size_t IndexForRemovedEntity = m_EntityToIndex[inEntity];
        size_t NewIndex = m_Size - 1; // get the last index of the array
        m_ComponentArray[NewIndex] = m_ComponentArray[IndexForRemovedEntity];

        // Update map to point to move the spot
        Entity EntityToRemove = m_IndexToEntity[IndexForRemovedEntity];
        m_EntityToIndex[EntityToRemove] = IndexForRemovedEntity;
        m_IndexToEntity[IndexForRemovedEntity] = EntityToRemove;

        m_EntityToIndex.erase(EntityToRemove);
        m_IndexToEntity.erase(IndexForRemovedEntity);

        --m_Size;
    }
    T& GetData(Entity inEntity)
    {
        return m_ComponentArray[m_EntityToIndex[inEntity]];
    }

    void OnEntityDestroyed(Entity inEntity) override
    {
        if (m_EntityToIndex.find(inEntity) != m_EntityToIndex.end())
        {
            RemoveData(inEntity);
        }
    }


private:
    std::array<T, MAX_ENTITIES> m_ComponentArray;
    std::unordered_map<Entity, size_t> m_EntityToIndex;
    std::unordered_map<size_t, Entity> m_IndexToEntity;

    // Total size of valid entries in the array.
    size_t m_Size = 0;
};

#endif //DUCKDUCKROAD_ICOMPONENTARRAY_H