#include "Shader.h"
#include "Utilities/GLErrorCatcher.h"


Shader::Shader(const std::string& _vsPath, const std::string& _fsPath)
    : m_RendererID(0)
{
    shaderUtils::GetSrcCode(_vsPath, m_vsSource);
    shaderUtils::GetSrcCode(_fsPath, m_fsSource);
    m_uniforms = shaderUtils::GetUniforms(_vsPath, _fsPath);

    m_RendererID = shaderUtils::CreateShader(m_vsSource, m_fsSource);
    cacheUniformLocations();
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::setUniform(const std::string& _uniformName, int _value)
{
    if(m_uniforms.iUniforms.find(_uniformName) != m_uniforms.iUniforms.end())
    {
        if(m_uniforms.iUniforms.at(_uniformName) != -1)
        {
            GLCall(glUniform1i(m_uniforms.iUniforms.at(_uniformName), _value));
        }
        else
        {
            std::cout << "No uniform var " << _uniformName << " found in compiled shader!" << std::endl;
        }
    }
    else
    {
        std::cout << "No uniform named "<< _uniformName << " found in shader!" << std::endl;;
    }
}

void Shader::setUniform(const std::string& _uniformName, float _value)
{
    if(m_uniforms.fUniforms.find(_uniformName) != m_uniforms.fUniforms.end())
    {
        if(m_uniforms.fUniforms.at(_uniformName) != -1)
        {
            GLCall(glUniform1f(m_uniforms.fUniforms.at(_uniformName), _value));
        }
        else
        {
            std::cout << "No uniform var " << _uniformName << " found in compiled shader!" << std::endl;
        }
    }
    else
    {
        std::cout << "No uniform named "<< _uniformName << " found in shader!" << std::endl;;
    }
}

void Shader::setUniform(const std::string& _uniformName, vec2 _value)
{
    if(m_uniforms.v2Uniforms.find(_uniformName) != m_uniforms.v2Uniforms.end())
    {
        if(m_uniforms.v2Uniforms.at(_uniformName) != -1)
        {
            GLCall(glUniform2f(m_uniforms.v2Uniforms.at(_uniformName), _value.x, _value.y));
        }
        else
        {
            std::cout << "No uniform var " << _uniformName << " found in compiled shader!" << std::endl;
        }
    }
    else
    {
        std::cout << "No uniform named "<< _uniformName << " found in shader!" << std::endl;;
    }
}

void Shader::setUniform(const std::string& _uniformName, vec3 _value)
{
    if(m_uniforms.v3Uniforms.find(_uniformName) != m_uniforms.v3Uniforms.end())
    {
        if(m_uniforms.v3Uniforms.at(_uniformName) != -1)
        {
            GLCall(glUniform3f(m_uniforms.v3Uniforms.at(_uniformName), _value.x, _value.y, _value.z));
        }
        else
        {
            std::cout << "No uniform var " << _uniformName << " found in compiled shader!" << std::endl;
        }
    }
    else
    {
        std::cout << "No uniform named "<< _uniformName << " found in shader!" << std::endl;;
    }
}

void Shader::setUniform(const std::string& _uniformName, vec4 _value)
{
    if(m_uniforms.v4Uniforms.find(_uniformName) != m_uniforms.v4Uniforms.end())
    {
        if(m_uniforms.v4Uniforms.at(_uniformName) != -1)
        {
            GLCall(glUniform4f(m_uniforms.v4Uniforms.at(_uniformName), _value.x, _value.y, _value.z, _value.a));
        }
        else
        {
            std::cout << "No uniform var " << _uniformName << " found in compiled shader!" << std::endl;
        }
    }
    else
    {
        std::cout << "No uniform named "<< _uniformName << " found in shader!" << std::endl;;
    }
}

GLint Shader::getUniformLocation(const std::string& name)
{
    return GLCall(glGetUniformLocation(m_RendererID, name.c_str()));
}

void Shader::cacheUniformLocations()
{
    //Ints
    for (auto& [varName, value] : m_uniforms.iUniforms)
    {
        value = getUniformLocation(varName);
    }
    //floats
    for (auto& [varName, value] : m_uniforms.fUniforms)
    {
        value = getUniformLocation(varName);
    }
    //vec2
    for (auto& [varName, value] : m_uniforms.v2Uniforms)
    {
        value = getUniformLocation(varName);
    }
    //vec3
    for (auto& [varName, value] : m_uniforms.v3Uniforms)
    {
        value = getUniformLocation(varName);
    }
    //vec4
    for (auto& [varName, value] : m_uniforms.v4Uniforms)
    {
        value = getUniformLocation(varName);
    }
}
