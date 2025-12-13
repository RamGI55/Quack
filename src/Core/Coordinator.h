//
// Created by PC on 13-Dec-25.
//

#ifndef DUCKDUCKROAD_COORDINATOR_H
#define DUCKDUCKROAD_COORDINATOR_H
#include "ComponentManager.h"
#include "EntityManager.h"
#include "SystemManager.h"


class Coordinator
{
public:
    void Init()
    {
        ComponentManager_   = std::make_unique<ComponentManager>();
        EntityManager_      = std::make_unique<EntityManager>();
        SystemManager_      = std::make_unique<SystemManager>();
    }

    // Entity Coordinators
    Entity CreateEntity()
    {
        return EntityManager_->CreateEntity();
    }

    void DestroyEntity(Entity inEntity)
    {
        EntityManager_->DestroyEntity(inEntity);
        ComponentManager_->OnEntityDestroyed(inEntity);
        SystemManager_->OnEntityDestroyed(inEntity);
    }

    // Component Coordinators
    template<typename T>
    void RegisterComponent()
    {
        ComponentManager_->RegisterComponent<T>();
    }

    template<typename T>
    void AddComponent(Entity inEntity, T inComponentType)
    {
        ComponentManager_->AddComponent<T>(inEntity, inComponentType);
        auto signature = EntityManager_->GetSignature(inEntity);
        signature.set(ComponentManager_->GetComponentType<T>(), true);
        EntityManager_->SetSignature(inEntity, signature);

        SystemManager_->OnEntitySignatureChanged(inEntity, signature);
    }

    template<typename T>
    void RemoveComponent(Entity inEntity)
    {
        ComponentManager_->RemoveComponent<T>(inEntity);
        auto signature = EntityManager_->GetSignature(inEntity);
        signature.set(ComponentManager_->GetComponentType<T>(), false);
        EntityManager_->SetSignature(inEntity, signature);

        SystemManager_->OnEntitySignatureChanged(inEntity, signature);
    }

    template<typename T>
    T& GetComponent (Entity inEntity)
    {
        return ComponentManager_->GetComponent<T>(inEntity);
    }

    template<typename T>
    ComponentType GetComponentType()
    {
        return ComponentManager_->GetComponentType<T>();
    }

    // System Coordinator
    template<typename T>
    std::shared_ptr<T> RegisterSystem()
    {
        return SystemManager_->RegisterSystem<T>();
    }

    template<typename T>
    void SetSystemSignature(Signature inSignature)
    {
        SystemManager_->SetSignature<T>(inSignature);
    }


private:
    std::unique_ptr<ComponentManager> ComponentManager_;
    std::unique_ptr<EntityManager> EntityManager_;
    std::unique_ptr<SystemManager> SystemManager_;
};


#endif //DUCKDUCKROAD_COORDINATOR_H