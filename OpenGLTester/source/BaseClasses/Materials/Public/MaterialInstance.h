#pragma once

#include "pch.h"
#include "Defines.h"
#include "ManagingClasses/Public/ShaderMachine.h"
#include "Utilities/Public/Utilities.h"

class Material;

class MaterialInstance final : public EngineInternal 
{
    friend Material;
public:
    MaterialInstance() = delete;
    
private:
    
    MaterialInstance(std::string _name, ShaderType _shaderType);

private:

    const ShaderType m_shader;
};
