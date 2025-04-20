#pragma once
#include "../../BaseClasses/Public/Shader.h"

class ShaderMachine
{
public:

    static ShaderMachine* get()
    {
        if(!m_instance)
        {
            m_instance = new ShaderMachine();
        }

        return m_instance;
    }

    bool setShader(ShaderType _shaderType);
    void resetShader();
    Shader* getShader(ShaderType _shaderType);
    ShaderType getSelectedShaderType() const {return m_selectedShader;}


private:
    ShaderMachine();
    
    void precompileShaders();
    
private:
    static ShaderMachine* m_instance;

    std::unordered_map<ShaderType, Shader*> m_shaders;
    ShaderType m_selectedShader = ShaderType::NONE;
};
