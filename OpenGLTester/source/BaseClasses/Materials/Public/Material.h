#pragma once

#include "pch.h"
#include "Defines.h"
#include "ManagingClasses/Public/ShaderMachine.h"
#include "Utilities/Public/Utilities.h"

//forward declarations
class MaterialLibrary;

//=================================================================================
//Wrapper+Interface class for holding all materials in one container
//=================================================================================
class Material
{
    friend MaterialLibrary;
    
public:

    virtual ~Material() {}
    //returns SPTR to underlying MaterialImpl (without the need to cast to specific MaterialImpl first)
    virtual SPTR<Material> getImpl() = 0; //might need default impl((

    //Getters:
    [[nodiscard]] uint32_t getUID() const {return m_id;}
    [[nodiscard]] const std::string& getName() const;
    [[nodiscard]] ShaderType getUnderlyingShaderType() const;

protected:
    Material() = delete;
    explicit Material(uint32_t _id, std::string _name) : m_id(_id), m_name(std::move(_name)) {}
    explicit Material(uint32_t _id, std::string _name, ShaderType _underlyingShader)
        : m_id(_id), m_name(std::move(_name)), m_underlyingShaderType(_underlyingShader) {}

    virtual bool bindMaterialShader() = 0; 
    
protected:
    
    const uint32_t m_id;
    std::string m_name;
    ShaderType m_underlyingShaderType = ShaderType::NONE;
};


//=================================================================================
//Singleton Material itself
//=================================================================================
template<ShaderType ShaderT>
class MaterialImpl : public Material
{
    friend MaterialLibrary;
    
public:

    static SPTR<Material> get()
    {
        if(!m_implInstance)
        {
            m_implInstance = SPTR<MaterialImpl<ShaderT>>(new MaterialImpl(ShaderT));
            std::cout << m_implInstance->getName() << " instanced" << std::endl;
        }
        return m_implInstance;
    }
    
    SPTR<Material> getImpl() override {return m_implInstance;}
    bool bindMaterialShader() override;
    
protected:
    
    explicit MaterialImpl(ShaderType = ShaderT);

private:
    
    MaterialImpl() = delete;
    
private:

    inline static SPTR<Material> m_implInstance = nullptr;
    
};

//=================================================================================
//Template implementations //todo: consider moving to separate .impl file
//=================================================================================
template <ShaderType ShaderT>
bool MaterialImpl<ShaderT>::bindMaterialShader()
{
    return ShaderMachine::get()->setShader(ShaderT);
}

template <ShaderType ShaderT>
MaterialImpl<ShaderT>::MaterialImpl(ShaderType)
:
    Material(utils::Utilities::getInternalUID(), 
        "Material_" + std::to_string(static_cast<int>(ShaderT)), //todo: change it after implementing enum to string conversions
        ShaderT)
{}
