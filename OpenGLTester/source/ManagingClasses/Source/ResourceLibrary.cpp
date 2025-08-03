#include "../Public/ResourceLibrary.h"

#include "BaseClasses/Public/Texture.h"
#include <filesystem>

namespace fs = std::filesystem;
typedef fs::recursive_directory_iterator Directory_Iterator;

void ResourceLibrary::init()
{
    if (m_isInitialized)
        return;
    
    m_isInitialized = true;

    //todo: textures and other resources require unique identifiers!
    //Atm two images with the same name, but different paths/extensions can't be imported correctly!
    Directory_Iterator directory_visitor_it = Directory_Iterator("resources/");
    for (const fs::directory_entry& dir_entry : directory_visitor_it)
    {
        if (dir_entry.is_directory() || !dir_entry.exists())
            continue; //we're only interested in existing files
        
        const std::string extension = dir_entry.path().extension().string();
        ResourceType found_resource_type = checkResourceType(extension);
        if (found_resource_type != ResourceType::NONE)
        {
            std::string resource_name = dir_entry.path().filename().string();
            resource_name.erase((resource_name.size() - extension.size()), extension.size()); //removing file extention
            
            m_resourcePaths[found_resource_type][resource_name] = dir_entry.path().relative_path().string();
            std::cout << "[ResourceLibrary::init]" << "Added resource " << resource_name
                    << " with path " << dir_entry.path().string() << '\n';
        }
    }
}

ResourceLibrary::ResourceType ResourceLibrary::checkResourceType(const std::string& _extension) const
{
    if (_extension == ".jpg" || _extension == ".jpeg" || _extension == ".png" || _extension == ".bmp")
    {
        return ResourceType::TEXTURE;
    }
    if (_extension == ".mp3" || _extension == ".wav" || _extension == ".ogg" || _extension == ".aif" || _extension == ".wma")
    {
        return ResourceType::SOUND;
    }
    //other checks might be added in the future

    return ResourceType::NONE;
}

SPTR<Texture> ResourceLibrary::getTexture(const std::string& _textureName)
{
    if (!m_isInitialized)
    {
        std::cout << "[ResourceLibrary::getTexture]" << "Not initialized!\n";
        return nullptr;
    }
    
    auto it_to_tex_path = m_resourcePaths[ResourceType::TEXTURE].find(_textureName);
    if (it_to_tex_path == m_resourcePaths[ResourceType::TEXTURE].end())
    {
        std::cerr << "Texture " << _textureName << " not found\n";
        return nullptr;
    }
    if (!m_cachedTextures.contains(_textureName))
    {
        //NOTE: can it be safely instanced this way without disrupting render? Consider making preload lists 
        m_cachedTextures.emplace(_textureName, M_SPTR<Texture>(it_to_tex_path->second));
    }

    //at the moment, trying to avoid multiple instances of the same texture, so we're keeping them as SPTR.
    //Batch renderer should avoid binding one texture to different slots, and other renderers are unlikely to use one texture twice in one draw call
    return m_cachedTextures[_textureName];
}

bool ResourceLibrary::hasTexture(const std::string& _textureName) const
{
    return m_cachedTextures.contains(_textureName) ||
            (m_resourcePaths.contains(ResourceType::TEXTURE) && m_resourcePaths.at(ResourceType::TEXTURE).contains(_textureName));
}
