#pragma once
#include "Defines.h"

class MaterialInstance;
class Quad;

class MeshesFactory
{
public:
    static MeshesFactory& get()
    {
        static MeshesFactory instance;
        return instance;
    }
    
    MeshesFactory(const MeshesFactory&) = delete;
    MeshesFactory& operator=(const MeshesFactory&) = delete;
    MeshesFactory(MeshesFactory&&) = delete;
    MeshesFactory& operator=(MeshesFactory&&) = delete;

    //Builds a flat Quad mesh with designated texture
    //If _useUniqueMatInst is set to false, returns a clone of first Mesh instanced with this texture with the same MaterialInst, disregarding passed _shaderType
    //todo: add blending settings, bool flag for stretching/tiling texture to quad size
    SPTR<Quad> buildQuad(const std::string& _textureName,
        ShaderType _shaderType,
        const SPTR<Node>& _parentNode = nullptr,
        bool _useUniqueMatInst = false,
        const std::string& _meshName = "", 
        const uvec2& _customSize = uvec2(0));

    bool hasTexture(const std::string& _textureName) const;
    
    
private:

    MeshesFactory() = default;
    ~MeshesFactory() = default;

private:

    std::unordered_map<std::string, SPTR<Quad>> m_cachedQuads; //std::string - texture name. Stores only Quads created with bool _useUniqueMatInst = false and _customSize = uvec2(0)
#ifdef USE_BATCH_RENDERER_PIPELINE
    std::unordered_map<ShaderType, SPTR<MaterialInstance>> m_cachedBatchMaterials;
#endif
};
