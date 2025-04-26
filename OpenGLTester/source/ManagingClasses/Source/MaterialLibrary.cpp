#include "../Public/MaterialLibrary.h"

MaterialLibrary::MaterialLibrary()
{
    //hardcoded list of built-in shaders
#include "Utilities/Source/MaterialList.inl"
}

MaterialLibrary& MaterialLibrary::get()
{
    if(!m_instance)
        m_instance = new MaterialLibrary;

    return *m_instance;
}

SPTR<Material> MaterialLibrary::getMaterial(ShaderType type)
{
    if(!m_materialLib.contains(type))
    {
        std::cerr << "[MaterialLibrary] Shader type " << std::to_string(CAST_I(type)) << " not found!" << std::endl;
        return nullptr;
    }
    
    return m_materialLib[type];
}
