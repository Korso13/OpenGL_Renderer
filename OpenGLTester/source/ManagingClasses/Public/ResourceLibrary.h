#pragma once
#include "Defines.h"

class Texture;

class ResourceLibrary
{
    enum class ResourceType : uint8_t
    {
        TEXTURE,
        SOUND, //to be added
        VFX, //to be added
        NONE
    };

    friend int main(); //todo: to be replaced with friend class EngineController, when it's ready
    typedef std::unordered_map<ResourceType, std::unordered_map<std::string, std::string>> ResourcePathsIndex;
    
public:

    static ResourceLibrary& get()
    {
        static ResourceLibrary instance;
        return instance;
    }
    
    ResourceLibrary(const ResourceLibrary&) = delete;
    ResourceLibrary& operator=(const ResourceLibrary&) = delete;
    ResourceLibrary(ResourceLibrary&&) = delete;
    ResourceLibrary& operator=(ResourceLibrary&&) = delete;
    
    [[nodiscard]] SPTR<Texture> getTexture(const std::string& _textureName);
    //todo: will add sound and vfx getters at the very least, probably wil move material getters here as well (in the future)
    
private:

    ResourceLibrary() = default;
    ~ResourceLibrary() = default;
    
    void init();
    ResourceType checkResourceType(const std::string& _extension) const;
    
private:

    bool m_isInitialized = false;
    std::unordered_map<std::string, SPTR<Texture>> m_cachedTextures;
    ResourcePathsIndex m_resourcePaths;
};
