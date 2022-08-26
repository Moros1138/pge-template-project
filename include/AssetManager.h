#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include "olcPixelGameEngine.h"
#include "olcSoundWaveEngine.h"

#include <cassert>
#include <map>
#include <string>
#include <vector>
#include <filesystem>

class AssetManager
{
private:
    AssetManager();
    ~AssetManager();

public:
    static AssetManager& getInstance()
    {
        static AssetManager The_ONE_and_ONLY_Instance;
        return The_ONE_and_ONLY_Instance;
    }

public: // Static methods

    // loads a graphic from the provided file path, maps it to the provided key
    static void LoadGraphic(const std::string& key, const std::string& path)
    {
        AssetManager& am = AssetManager::getInstance();
        am._loadGraphic(key, path);
    }
    
    // loads a sound from the provided file path, maps it to the provided key
    static void LoadSound(const std::string& key, const std::string& path)
    {
        AssetManager& am = AssetManager::getInstance();
        am._loadSound(key, path);
    }

    // get the decal of the provided key
    static olc::Decal* GetDecal(const std::string& key)
    {
        AssetManager& am = AssetManager::getInstance();
        return am._getGraphic(key)->Decal();
    }
    
    // get the sprite of the provided key
    static olc::Sprite* GetSprite(const std::string& key)
    {
        AssetManager& am = AssetManager::getInstance();
        return am._getGraphic(key)->Sprite();
    }

    // get the olc::Renderable of the provided key
    static olc::Renderable* GetRenderable(const std::string& key)
    {
        AssetManager& am = AssetManager::getInstance();
        return am._getGraphic(key);
    }

    // get the olc::sound::Wave of the provided key
    static olc::sound::Wave* GetSound(const std::string& key)
    {
        AssetManager& am = AssetManager::getInstance();
        return am._getSound(key);
    }

public: // Non-static methods
    olc::Renderable* _getGraphic(const std::string& key);
    olc::sound::Wave* _getSound(const std::string& key);
    void _loadGraphic(const std::string& key, const std::string& path);
    void _loadSound(const std::string& key, const std::string& path);

private: // Non-static properties
    std::map<std::string, olc::sound::Wave*> mSounds;
    std::map<std::string, olc::Renderable*> mGraphics;

}; // AssetManager

#ifdef ASSET_MANAGER_IMPLEMENTATION
#undef ASSET_MANAGER_IMPLEMENTATION

AssetManager::AssetManager()
{ }

AssetManager::~AssetManager()
{
    for(auto & g : mGraphics) delete g.second;
    for(auto & s : mSounds)   delete s.second;
    
    mGraphics.clear();
    mSounds.clear();
}

olc::Renderable* AssetManager::_getGraphic(const std::string& key)
{
    
    assert(mGraphics.find(key) != mGraphics.end());

    return mGraphics[key];
}

olc::sound::Wave* AssetManager::_getSound(const std::string& key)
{
    assert(mSounds.find(key) != mSounds.end());

    return mSounds[key];
}

void AssetManager::_loadGraphic(const std::string& key, const std::string& path)
{
    // ensure we have actually set a key and path
    assert(!key.empty() && !path.empty());
    
    // check if the provided key already exists!
    if(mGraphics.find(key) != mGraphics.end())
    {
        std::cout << "AssetManager: attempted to use an a key <" << key << "> that already existed." << std::endl;
        exit(EXIT_FAILURE);
    }

    // ensure the file exists and is readable
    std::filesystem::path assetPath = std::filesystem::weakly_canonical(std::filesystem::absolute(path));
    if(!(std::filesystem::exists(assetPath) && std::filesystem::is_regular_file(assetPath)))
    {
        std::cout << "AssetManager: attempted to load file <" << path << "> which does not exist." << std::endl;
        exit(EXIT_FAILURE);
    }

    olc::Renderable* graphic = new olc::Renderable();
    graphic->Load(path);
    
    if(graphic->Sprite() == nullptr || graphic->Decal() == nullptr)
    {
        std::cout << "AssetManager: attempted to load sprite <" << path << ">, and something went wrong, is it a valid PNG file?" << std::endl;
        exit(EXIT_FAILURE);
    }

    mGraphics[key] = graphic;
}

void AssetManager::_loadSound(const std::string& key, const std::string& path)
{
    // ensure we have actually set a key and path
    assert(!key.empty() && !path.empty());
    
    // check if the provided key already exists!
    if(mSounds.find(key) != mSounds.end())
    {
        std::cout << "AssetManager: attempted to use an a key <" << key << "> that already existed." << std::endl;
        exit(EXIT_FAILURE);
    }

    // ensure the file exists and is readable
    std::filesystem::path assetPath = std::filesystem::weakly_canonical(std::filesystem::absolute(path));
    if(!(std::filesystem::exists(assetPath) && std::filesystem::is_regular_file(assetPath)))
    {
        std::cout << "AssetManager: attempted to load file <" << path << "> which does not exist." << std::endl;
        exit(EXIT_FAILURE);
    }

    olc::sound::Wave* sample = new olc::sound::Wave(path);

    if(sample == nullptr)
    {
        std::cout << "AssetManager: attempted to load sound <" << path << ">, and something went wrong, is it a valid WAV file?" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    mSounds[key] = sample;
}

#endif // ASSET_MANAGER_IMPLEMENTATION

#endif // ASSET_MANAGER_H