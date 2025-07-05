#pragma once

#include "pch.h"
#include "Defines.h"
#include "ManagingClasses/Public/Renderer.h"
#include "ManagingClasses/Public/ShaderMachine.h"
#include "Utilities/Public/Utilities.h"

//using for more readable variable_uniform
using intVectorParam = std::array<int, MAX_RENDER_BATCH_SIZE>;
//std::variant holding shader uniform parameters
using variable_uniform = std::variant<bool, int, float, vec2, vec3, vec4, /*ivec2,*/ glm::mat4, intVectorParam>;
//concept declaration limiting the use of setUniformParam(), getUniformParam() and hasUniformParam()
template<typename T>
concept UniformParam = requires(T _param, variable_uniform _uniform_param)
{
    _uniform_param = _param;
};

class Texture;
class Material;

class MaterialInstance final : public EngineInternal 
{
    friend Material;
    friend Renderer;
    
public:
    MaterialInstance() = delete;

    //Shader uniforms methods
    template<UniformParam T>
    void setUniformParam(const std::string& _paramName, T _param);
    template<UniformParam T>
    T getUniformParam(const std::string& _paramName) const;
    template<UniformParam T>
    [[nodiscard]] bool hasUniformParam(const std::string& _paramName) const;

    //Work with textures
    [[nodiscard]] bool canHoldMoreTextures();
    [[nodiscard]] TextureId getAvailableTextureId();
    //sets texture to a specific slot. Slot == float(int) TextureID that you need to set to the vertices of your mesh
    void setTextureToSlot(SPTR<Texture> _texture, TextureId _slotId);
    //sets texture to first available slot and returns said slot. Slot == float(int) TextureID that you need to set to the vertices of your mesh
    TextureId setTexture(SPTR<Texture> _texture);
    SPTR<Texture> getTexture(TextureId _slotId) const;
    bool removeTexture(TextureId _slotId);

    ShaderType getShaderType() const{return m_shaderType;}
    size_t getTexturesCount() const {return m_textures.size();}
    
private:
    
    MaterialInstance(std::string _name, ShaderType _shaderType);

    void inputUniformParamsToShader();
    void setMVP(const glm::mat4& _mvp);
    void checkUnusedTextureSlots();
    //todo: implement fast checking of repeated textures and use it in setTexture method in the beginning
    void checkIfAlreadyUsesTexture(const SPTR<Texture> _texture);
    
private:

    const ShaderType m_shaderType;
    SPTR<Shader> m_shader = nullptr;
    std::map<TextureId, SPTR<Texture>> m_textures; //different RenderObjects may use the same MaterialInst, but have different textures
    std::unordered_map<std::string, variable_uniform> m_materialParams; //list of shader settings
};

//Template definitions
#include "../source/MaterialInstance.tpp"
