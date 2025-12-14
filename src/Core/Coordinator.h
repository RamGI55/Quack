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
        m_ComponentManager   = std::make_unique<ComponentManager>();
        m_EntityManager      = std::make_unique<EntityManager>();
        m_SystemManager      = std::make_unique<SystemManager>();
    }

    // Entity Coordinators
    [[nodiscard]] Entity CreateEntity() const
    {
        return m_EntityManager->CreateEntity();
    }

    void DestroyEntity(const Entity inEntity) const
    {
        m_EntityManager->DestroyEntity(inEntity);
        m_ComponentManager->OnEntityDestroyed(inEntity);
        m_SystemManager->OnEntityDestroyed(inEntity);
    }

    // Component Coordinators
    template<typename T>
    void RegisterComponent() const
    {
        m_ComponentManager->RegisterComponent<T>();
    }

    template<typename T>
    void AddComponent(const Entity inEntity, T inComponentType)
    {
        m_ComponentManager->AddComponent<T>(inEntity, inComponentType);
        auto signature = m_EntityManager->GetSignature(inEntity);
        signature.set(m_ComponentManager->GetComponentType<T>(), true);
        m_EntityManager->SetSignature(inEntity, signature);

        m_SystemManager->OnEntitySignatureChanged(inEntity, signature);
    }

    template<typename T>
    void RemoveComponent(const Entity inEntity) const
    {
        m_ComponentManager->RemoveComponent<T>(inEntity);
        auto signature = m_EntityManager->GetSignature(inEntity);
        signature.set(m_ComponentManager->GetComponentType<T>(), false);
        m_EntityManager->SetSignature(inEntity, signature);

        m_SystemManager->OnEntitySignatureChanged(inEntity, signature);
    }

    template<typename T>
    T& GetComponent (Entity inEntity)
    {
        return m_ComponentManager->GetComponent<T>(inEntity);
    }

    template<typename T>
    [[nodiscard]] ComponentType GetComponentType() const
    {
        return m_ComponentManager->GetComponentType<T>();
    }

    // System Coordinator
    template<typename T>
    std::shared_ptr<T> RegisterSystem()
    {
        return m_SystemManager->RegisterSystem<T>();
    }

    template<typename T>
    void SetSystemSignature(const Signature inSignature) const
    {
        m_SystemManager->SetSignature<T>(inSignature);
    }


private:
    std::unique_ptr<ComponentManager> m_ComponentManager;
    std::unique_ptr<EntityManager> m_EntityManager;
    std::unique_ptr<SystemManager> m_SystemManager;
};


#endif //DUCKDUCKROAD_COORDINATOR_H