#include "../Public/MeshesFactory.h"

#include "BaseClasses/Public/Texture.h"
#include "BaseClasses/RenderObjects/Public/Quad.h"
#include "ManagingClasses/Public/MaterialLibrary.h"
#include "ManagingClasses/Public/ResourceLibrary.h"

SPTR<Quad> MeshesFactory::buildQuad(
    const std::string& _textureName,
    ShaderType _shaderType,
    const SPTR<Node>& _parentNode,
    bool _useUniqueMatInst,
    const std::string& _meshName,
    const uvec2& _customSize)
{
    if (!hasTexture(_textureName))
    {
        std::cerr << "[MeshesFactory::buildQuad]" << "Texture " << _textureName << " not found!\n";
        return nullptr;
    }
    
    //Non-unique materialInst, looking for a cached Quad
    if (!_useUniqueMatInst && m_cachedQuads.contains(_textureName))
    {
        SPTR<Quad> new_quad_mesh = m_cachedQuads[_textureName]->clone<Quad>();
        if (!_meshName.empty())
            new_quad_mesh->setName(_meshName);
        if (_parentNode)
            _parentNode->addChild(new_quad_mesh);
        return new_quad_mesh;
    }

    if (_shaderType == ShaderType::NONE)
    {
        std::cerr << "[MeshesFactory::buildQuad]" << "Can't instance material with NONE shader type and no cached mesh is requested / found!\n";
        return nullptr;
    }

    const SPTR<Texture> tex = RL_TEXTURE(_textureName);
    auto new_quad_mesh = Quad::build(
        (_customSize != uvec2(0) ? _customSize : uvec2(tex->getSize())), //using texture's size for Quad size, if _customSize is not valid
        vec3(0.f),
        (_meshName.empty() ? _textureName : _meshName)
    );
    if (!new_quad_mesh)
    {
        std::cerr << "[MeshesFactory::buildQuad]" << "Quad build error!\n";
        return nullptr;
    }
    //Setting up material for new mesh (no check for _useUniqueMatInst needed - if there was a cached MatInst, we would have already returned cloned mesh)
    const SPTR<MaterialInstance> mat_inst = MAT_LIB.getMaterial(_shaderType)->createInstance();
    mat_inst->setTexture(tex);
    new_quad_mesh->setMatInst(mat_inst);

    //caching new non-unique mesh
    if (!_useUniqueMatInst && !m_cachedQuads.contains(_textureName))
    {
        m_cachedQuads[_textureName] = new_quad_mesh;
        SPTR<Quad> quad_clone = new_quad_mesh->clone<Quad>();
        m_cachedQuads[_textureName]->setName(_textureName);
        if (!_meshName.empty())
            quad_clone->setName(_meshName);
        if (_parentNode)
            _parentNode->addChild(quad_clone);
        
        return quad_clone;
    }
    
    if (_parentNode)
        _parentNode->addChild(new_quad_mesh);
    
    return new_quad_mesh;
}

bool MeshesFactory::hasTexture(const std::string& _textureName) const
{
    return RES_LIB.hasTexture(_textureName);
}
