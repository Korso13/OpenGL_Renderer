#include <utility>

#include "../Public/MaterialInstance.h"

#include "BaseClasses/Public/Texture.h"

MaterialInstance::MaterialInstance(std::string _name, ShaderType _shaderType)
    :
    EngineInternal(std::move(_name)),
    m_shaderType(_shaderType),
    m_shader(ShaderMachine::get()->getShader(_shaderType))
{
}

bool MaterialInstance::canHoldMoreTextures()
{
    checkUnusedTextureSlots();
    return CAST_I(m_textures.size()) < Renderer::get()->getMaxBatchSize();
}

TextureId MaterialInstance::getAvailableTextureId()
{
    if(!canHoldMoreTextures())
        return TextureId::SIZE;

    TextureId result = static_cast<TextureId>(m_textures.size()); //fast slot id guess
    if(m_textures.contains(result)) //longer search in case we guessed wrong
    {
        for (int i = 0; i < Renderer::get()->getMaxBatchSize(); ++i)
        {
            result = static_cast<TextureId>(i);
            if(!m_textures.contains(result))
                break;
        }
    }

    return result;
}

void MaterialInstance::setTextureToSlot(const SPTR<Texture> _texture, TextureId _slotId)
{
    m_textures[_slotId] = _texture;
    if (!m_materialParams.contains("u_Textures"))
    {
        m_materialParams["u_Textures"] = intVectorParam();
    }
    get<intVectorParam>(m_materialParams["u_Textures"])[CAST_I(_slotId)] = CAST_I(_slotId);
}

TextureId MaterialInstance::setTexture(const SPTR<Texture> _texture)
{
    assert(_texture && "[MaterialInstance::setTexture]Texture is null");
    
    //checking if material instance already has such texture
    const TextureId existing_texture = checkIfAlreadyUsesTexture(_texture);
    if (existing_texture != TextureId::SIZE)
        return existing_texture;

    const TextureId candidate_slot = getAvailableTextureId();
    if(candidate_slot == TextureId::SIZE)
    {
        std::cerr << "[MaterialInstance::setTexture] Not enough slots!\n";
        return candidate_slot;
    }

    setTextureToSlot(_texture, candidate_slot);
    return candidate_slot;
}

SPTR<Texture> MaterialInstance::getTexture(const TextureId _slotId) const
{
    const auto It = m_textures.find(_slotId);
    if(It == m_textures.end())
        return nullptr;
    else
        return (*It).second;
}

bool MaterialInstance::removeTexture(TextureId _slotId)
{
    if(!m_textures.contains(_slotId))
        return false;

    m_textures.erase(_slotId);
    get<intVectorParam>(m_materialParams["u_Textures"])[CAST_I(_slotId)] = -1;
    return true;
}

void MaterialInstance::inputUniformParamsToShader()
{
    ASSERT(m_shader)
    //if(ShaderMachine::get()->getSelectedShaderType() != m_shaderType)
        ASSERT(ShaderMachine::get()->setShader(m_shaderType))

    //binding MaterialInst textures to specified slots 
    for (const auto& [textureId, texture] : m_textures)
    {
        texture->bind(CAST_I(textureId));
    }
    
    for (auto [paramName, param] : m_materialParams)
    {
        //todo: consider caching Visitor for performance improvement
        std::visit(
            Visitor{
                [this, paramName](intVectorParam& _var)
                {
                    m_shader->setUniform(paramName, _var.data(), Renderer::get()->getMaxBatchSize());
                },
                [this, paramName](const bool _var)
                {
                    m_shader->setUniform(paramName, _var);
                },
                [this, paramName](const int _var)
                {
                    m_shader->setUniform(paramName, _var);
                },
                [this, paramName](const float _var)
                {
                    m_shader->setUniform(paramName, _var);
                },
                [this, paramName](const vec2& _var)
                {
                    m_shader->setUniform(paramName, _var);
                },
                [this, paramName](const vec3 _var)
                {
                    m_shader->setUniform(paramName, _var);
                },
                [this, paramName](const vec4 _var)
                {
                    m_shader->setUniform(paramName, _var);
                },
                [this, paramName](const glm::mat4 _var)
                {
                    m_shader->setUniform(paramName, _var);
                }
            },
            param
        );
    }
}

void MaterialInstance::setMVP(const glm::mat4& _mvp)
{
    ASSERT(m_shader)
    //if(ShaderMachine::get()->getSelectedShaderType() != m_shaderType)
        ASSERT(ShaderMachine::get()->setShader(m_shaderType))
    m_shader->setUniform("u_MVP", _mvp);
}

void MaterialInstance::checkUnusedTextureSlots()
{
    std::erase_if(m_textures, [this](std::pair<const TextureId,std::shared_ptr<Texture>> _texturRecord)
    {
        return !_texturRecord.second;
    });
}

TextureId MaterialInstance::checkIfAlreadyUsesTexture(const SPTR<Texture>& _checkedTexture) const
{
    for (const auto& [slotId, texture] : m_textures)
        if (texture->getUID() == _checkedTexture->getUID())
            return slotId;
    
    return TextureId::SIZE;
}
