#include "../Public/ShaderMachine.h"
#include "Utilities/Public/ShaderLib.h"

ShaderMachine* ShaderMachine::m_instance = nullptr;

ShaderMachine::ShaderMachine()
{
    precompileShaders();
}

bool ShaderMachine::setShader(ShaderType _shaderType)
{
    if(m_shaders.find(_shaderType) == m_shaders.end())
        return false;
    else
    {
        if(m_selectedShader == _shaderType)
            return true; //for optimization purposes
        
        m_shaders.at(_shaderType)->bind();
        m_selectedShader = _shaderType;
        return true;
    }
}

void ShaderMachine::resetShader()
{
    if(m_selectedShader == ShaderType::NONE)
        return;
    else
    {
        m_shaders[m_selectedShader]->unbind();
    }
}

Shader* ShaderMachine::getShader(ShaderType _shaderType)
{
    if(m_shaders.find(_shaderType) == m_shaders.end())
        return nullptr;
    else
        return m_shaders.at(_shaderType);
}

void ShaderMachine::precompileShaders()
{
    for(const auto& [ShaderType, Paths] : ShaderLib::get())
    {
        Shader* shader = new Shader(Paths.first, Paths.second);
        m_shaders.emplace(ShaderType, shader);
    }
}
