//
// Created by PC on 13-Dec-25.
//

#ifndef DUCKDUCKROAD_COMPONENTMANAGER_H
#define DUCKDUCKROAD_COMPONENTMANAGER_H
#include <cassert>
#include <memory>
#include <typeinfo>
#include <unordered_map>

#include "IComponentArray.h"
#include "Types.h"

class ComponentManager
{
public:
    template<typename T>
    void RegisterComponent()
    {
        const char* TypeName= typeid(T).name();

        assert(mComponentTypes_.find(TypeName) == mComponentTypes_.end() && "The Component Type has more than one types");

        mComponentTypes_.insert({TypeName, NextComponentType_});

        mComponentArrays_.insert({TypeName, std::make_shared<ComponentArray<T>>()});

        ++NextComponentType_;
    }

    template<typename T>
    ComponentType GetComponentType()
    {
        const char* TypeName = typeid(T).name();

        assert(mComponentTypes_.find(TypeName) != mComponentTypes_.end() && "Cannot Find the Component.");

        return mComponentTypes_[TypeName];
    }

    template <typename T>
    void AddComponent(Entity inEntity, T inComponent)
    {
        GetComponentArray<T>()->InsertData(inEntity, inComponent);
    }

    template <typename T>
    void RemoveComponent(Entity inEntity)
    {
        GetComponentArray<T>()->RemoveData(inEntity);
    }

    template <typename T>
    T& GetComponent(Entity inEntity)
    {
        return GetComponentArray<T>()->GetData(inEntity);
    }

    void OnEntityDestroyed(Entity inEntity) const
    {
        for (auto const& pair : mComponentArrays_)
        {
            auto const& component = pair.second;

            component->OnEntityDestroyed(inEntity);
        }
    }

private:
    std::unordered_map<const char*, ComponentType> mComponentTypes_;
    std::unordered_map<const char*, std::shared_ptr<IComponentArray>> mComponentArrays_;
    ComponentType NextComponentType_ = 0;

    template<typename T>
    std::shared_ptr<ComponentArray<T>> GetComponentArray()
    {
        const char* TypeName = typeid(T).name();

        assert(mComponentTypes_.find(TypeName) != mComponentTypes_.end() && "Cannot Find the Component.");

        // returns the pointer from the const char array representation of a Type T => using it as the unique key into a map of Component Types.
        return std::static_pointer_cast<ComponentArray<T>>(mComponentArrays_[TypeName])();

    }
};


#endif //DUCKDUCKROAD_COMPONENTMANAGER_H