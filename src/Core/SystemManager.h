//
// Created by PC on 13-Dec-25.
//

#ifndef DUCKDUCKROAD_SYSTEMMANAGER_H
#define DUCKDUCKROAD_SYSTEMMANAGER_H
#include <cassert>
#include <memory>
#include <unordered_map>

#include "System.h"
#include "Types.h"


class SystemManager
{
public:
    template<typename T>
    std::shared_ptr<T> RegisterSystem()
    {
        const char* TypeName = typeid(T).name();

        assert(SystemsMap_.find(TypeName) == SystemsMap_.end() && "The system has already been registered.");

        auto system = std::make_shared<T>(this);
        SystemsMap_.insert({TypeName, system});

        return system;
    }

    template<typename T>
    void SetSignature(Signature inSignature)
    {
        const char* TypeName = typeid(T).name();

        assert(SystemsMap_.find(TypeName) != SystemsMap_.end() && "Ths System used before registered");

        SignaturesMap_.insert({TypeName, inSignature});
    }

    void OnEntityDestroyed(Entity entity)
    {
        for (auto const& pair : SystemsMap_)
        {
            auto const& system = pair.second;
            system->Entities.erase(entity);
        }
    }

    void OnEntitySignatureChanged(const Entity inEntity, const Signature inEntitySignature)
    {
        for (auto const& pair : SystemsMap_)
        {
            auto const& Type = pair.first;
            auto const& System = pair.second;
            auto const& SystemSignature = SignaturesMap_[Type];

            if ((inEntitySignature & SystemSignature) == SystemSignature)
            {
                System->Entities.insert(inEntity);
            }
            else
            {
                System->Entities.erase(inEntity);
            }
        }
    }


private:
    // string pointer -> Signature
    std::unordered_map<const char*, Signature> SignaturesMap_;
    // String pointer -> Systems
    std::unordered_map<const char*, std::shared_ptr<System>> SystemsMap_;
};


#endif //DUCKDUCKROAD_SYSTEMMANAGER_H