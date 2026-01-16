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

        assert(m_ComponentTypes_.find(TypeName) == m_ComponentTypes_.end() && "The Component Type has more than one types");

        m_ComponentTypes_.insert({TypeName, m_NextComponentType_});
        m_ComponentArrays_.insert({TypeName, std::make_shared<ComponentArray<T>>()});
        ++m_NextComponentType_;
    }

    template<typename T>
    ComponentType GetComponentType()
    {
        const char* TypeName = typeid(T).name();

        assert(m_ComponentTypes_.find(TypeName) != m_ComponentTypes_.end() && "Cannot Find the Component.");

        return m_ComponentTypes_[TypeName];
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

    void OnEntityDestroyed(Entity inEntity)
    {
        for (auto const& pair : m_ComponentArrays_)
        {
            auto const& component = pair.second;

            component->OnEntityDestroyed(inEntity);
        }
    }

private:
    std::unordered_map<const char*, ComponentType> m_ComponentTypes_;
    std::unordered_map<const char*, std::shared_ptr<IComponentArray>> m_ComponentArrays_;
    ComponentType m_NextComponentType_ = 0;

    template<typename T>
    std::shared_ptr<ComponentArray<T>> GetComponentArray()
    {
        const char* TypeName = typeid(T).name();

        assert(m_ComponentTypes_.find(TypeName) != m_ComponentTypes_.end() && "Cannot Find the Component.");

        // returns the pointer from the const char array representation of a Type T => using it as the unique key into a map of Component Types.
        return std::static_pointer_cast<ComponentArray<T>>(m_ComponentArrays_[TypeName]);

    }
};


#endif //DUCKDUCKROAD_COMPONENTMANAGER_H