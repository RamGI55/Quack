//
// Created by Hyunsoo Park on 12/27/25.
//

#ifndef DUCKDUCKROAD_ASSETMANAGER_H
#define DUCKDUCKROAD_ASSETMANAGER_H
#include <any>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

#include "../../cmake-build-debug/_deps/sfml-src/src/SFML/System/Utils.hpp"
#include "SFML/Audio/Music.hpp"
#include "SFML/Audio/SoundBuffer.hpp"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/Texture.hpp"


class AssetManager
{

private:
    std::unordered_map<const char*, std::any> mCaches;

    template<typename T>
    using CacheMap = std::unordered_map<std::string, std::shared_ptr<T>>;

    template<typename T>
    CacheMap<T>& GetCache()
    {
        const char* typeName = typeid(T).name();

        if (mCaches.find(typeName) == mCaches.end())
        {
            mCaches[typeName] = CacheMap<T>(); // TODO: Think again it is good idea to make another cache map if it doesn't have.
        }

        return std::any_cast<CacheMap<T>&>(mCaches[typeName]);
    }

public:
    template<typename T>
    bool Load(const std::string& id, const std::string& FilePath);

    template<typename T>
    std::shared_ptr<T> Get(std::string id)
    {
        auto& cache = GetCache<T>();
        auto it = cache.find(id);

        if (it == cache.end())
        {
            std::cerr << "[AssetManager] Asset not found: " << id << "\n";
            return GetFallback<T>();
        }
        return it->second;
    }

    template<typename T>
    bool Has(std::string id)
    {
        auto& cache = GetCache<T>();
        return cache.find(id) != cache.end();
    }

    template<typename T>
    void Unload(std::string id)
    {
        GetCache<T>().erase(id);
    }

    template<typename T>
    void Clear()
    {
        const char* typeName = typeid(T).name();
        mCaches.erase(typeName);
    }

    void ClearALl()
    {
        mCaches.clear();
    }

private:
    template<typename T>
    std::shared_ptr<T> GetFallback()
    {
        std::cerr << "[AssetManager] Asset not found: " << typeid(T).name() << "\n";
    }

};

    template<>
    inline bool AssetManager::Load<sf::Texture>(const std::string& id, const std::string& FilePath)
    {
        auto asset = std::make_shared<sf::Texture>();
        if (!asset->loadFromFile(FilePath))
        {
            std::cerr << "[AssetManager] Asset loading failed: " << FilePath << "\n";
            return false;
        }
        GetCache<sf::Texture>()[id] = asset;
        return true;

    }

    template<>
    inline bool AssetManager::Load<sf::SoundBuffer>(const std::string& id, const std::string& FilePath)
    {
        auto asset = std::make_shared<sf::SoundBuffer>();
        if (!asset->loadFromFile(FilePath))
        {
            std::cerr << "[AssetManager] Asset loading failed: " << FilePath << "\n";
            return false;
        }
        GetCache<sf::SoundBuffer>()[id] = asset;
        return true;
    }

    template<>
    inline bool AssetManager::Load<sf::Font>(const std::string& id, const std::string& FilePath)
    {
        auto asset = std::make_shared<sf::Font>();
        if (!asset->openFromFile(FilePath))
        {
            std::cerr << "[AssetManager] Asset loading failed: " << FilePath << "\n";
            return false;
        }
        GetCache<sf::Font>()[id] = asset;
        return true;
    }


#endif //DUCKDUCKROAD_ASSETMANAGER_H