#pragma once

#include "pch.h"
#include "Defines.h"
#include "ManagingClasses/Public/ShaderMachine.h"
#include "Utilities/Public/Utilities.h"

class MaterialInstance;
//forward declarations
class MaterialLibrary;

//=================================================================================
//Wrapper+Interface class for holding all materials in one container
//=================================================================================
class Material : public EngineInternal
{
    friend MaterialLibrary;
    
public:

    virtual ~Material() {}
    //returns SPTR to underlying MaterialImpl (without the need to cast to specific MaterialImpl first)
    virtual SPTR<Material> getImpl() = 0; //might need default impl((

    virtual SPTR<MaterialInstance> createInstance();
    
    //Getters:
    [[nodiscard]] ShaderType getUnderlyingShaderType() const;

protected:
    Material() = delete;
    explicit Material(std::string _name) : EngineInternal(std::move(_name)) {}
    explicit Material(std::string _name, ShaderType _underlyingShader)
        : EngineInternal(std::move(_name)), m_underlyingShaderType(_underlyingShader) {}

    virtual bool bindMaterialShader() = 0; 
    
protected:
    
    ShaderType m_underlyingShaderType = ShaderType::NONE;
};


//=================================================================================
//Singleton Material itself
//=================================================================================
template<ShaderType ShaderT>
class MaterialImpl final : public Material
{
    friend MaterialLibrary;
    
public:

    constexpr static SPTR<Material> get()
    {
        if(!m_implInstance)
        {
            m_implInstance = SPTR<MaterialImpl<ShaderT>>(new MaterialImpl(ShaderT));
            std::cout << m_implInstance->getName() << " instanced" << std::endl;
        }
        return m_implInstance;
    }
    
    SPTR<Material> getImpl() override {return m_implInstance;}
    
protected:
    
    explicit MaterialImpl(ShaderType = ShaderT);
    bool bindMaterialShader() override;

private:
    
    MaterialImpl() = delete;
    
private:

    inline static SPTR<Material> m_implInstance = nullptr;
    
};

//=================================================================================
//Template implementations //todo: consider moving to separate .tpp file
//=================================================================================
template <ShaderType ShaderT>
bool MaterialImpl<ShaderT>::bindMaterialShader()
{
    return ShaderMachine::get()->setShader(ShaderT);
}

template <ShaderType ShaderT>
MaterialImpl<ShaderT>::MaterialImpl(ShaderType)
:
    Material("Material_" + std::to_string(static_cast<int>(ShaderT)), //todo: change it after implementing enum to string conversions
            ShaderT)
{}
