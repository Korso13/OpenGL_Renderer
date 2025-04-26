#pragma once
#include "pch.h"
#include "Defines.h"
#include "BaseClasses/Materials/Public/Material.h"

class Material;

class MaterialLibrary
{
public:

    static MaterialLibrary& get();

    SPTR<Material> getMaterial(ShaderType type);

    //auxiliary method for registering new Materials, if they are not added to the Utilities/Source/MaterialList.impl
    template<ShaderType ShaderT>
    SPTR<Material> registerNonDefaultMaterial()
    {
        m_materialLib.insert({ShaderT, MaterialImpl<ShaderT>::get()});
        return m_materialLib[ShaderT];
    }
    
private:
    MaterialLibrary();
    ~MaterialLibrary() = default;

private:

    inline static MaterialLibrary* m_instance = nullptr;
    std::unordered_map<ShaderType, SPTR<Material>> m_materialLib;
};
