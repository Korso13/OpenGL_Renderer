#include "../Public/Shader.h"
#include "Utilities/Public/GLErrorCatcher.h"
#include "Utilities/Public/ShaderUtilities.h"


Shader::Shader(const std::string& _vsPath, const std::string& _fsPath)
    : m_RendererID(0)
{
    std::string vsSource;
    std::string fsSource;
    shaderUtils::GetSrcCode(_vsPath, vsSource);
    shaderUtils::GetSrcCode(_fsPath, fsSource);
    m_uniforms = shaderUtils::GetUniforms(_vsPath, _fsPath);

    m_RendererID = shaderUtils::CreateShader(vsSource, fsSource);
    bind();
    cacheUniformLocations();
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
    m_uniforms.fUniforms.clear();
    m_uniforms.iUniforms.clear();
    m_uniforms.ivUniforms.clear();
    m_uniforms.v2Uniforms.clear();
    m_uniforms.v3Uniforms.clear();
    m_uniforms.v4Uniforms.clear();
}

void Shader::bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::unbind() const
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
            GLint loc = getUniformLocation(_uniformName);
            if(loc == -1)
                std::cout << "No uniform var " << _uniformName << " found in compiled shader!" << std::endl;
            else
            {
                std::cout << "Uniform var " << _uniformName << " found in compiled shader after 2nd attempt!" << std::endl;
            }
        }
    }
    else
    {
        std::cout << "No uniform named "<< _uniformName << " found in shader!" << std::endl;
    }
}

void Shader::setUniform(const std::string& _uniformName, bool _value)
{
    if(m_uniforms.iUniforms.find(_uniformName) != m_uniforms.iUniforms.end())
    {
        if(m_uniforms.iUniforms.at(_uniformName) != -1)
        {
            GLCall(glUniform1i(m_uniforms.iUniforms.at(_uniformName), CAST_I(_value)));
        }
        else
        {
            GLint loc = getUniformLocation(_uniformName);
            if(loc == -1)
                std::cout << "No uniform var " << _uniformName << " found in compiled shader!" << std::endl;
            else
            {
                std::cout << "Uniform var " << _uniformName << " found in compiled shader after 2nd attempt!" << std::endl;
            }
        }
    }
    else
    {
        std::cout << "No uniform named "<< _uniformName << " found in shader!" << std::endl;
    }
}

void Shader::setUniform(const std::string& _uniformName, int* _value, int count)
{
    if(m_uniforms.ivUniforms.find(_uniformName) != m_uniforms.ivUniforms.end())
    {
        if(m_uniforms.ivUniforms.at(_uniformName).second < count)
        {
            std::cout << "Passed too many values for uniform " << _uniformName << "!" << count << " vs " << m_uniforms.ivUniforms.at(_uniformName).second << " available in shader\n";
            return;
        }
        
        if(m_uniforms.ivUniforms.at(_uniformName).first != -1)
        {
            GLCall(glUniform1iv(m_uniforms.ivUniforms.at(_uniformName).first, count, _value));
        }
        else
        {
            GLint loc = getUniformLocation(_uniformName);
            if(loc == -1)
                std::cout << "No uniform var " << _uniformName << " for int arrays found in compiled shader!" << std::endl;
            else
            {
                std::cout << "Uniform var " << _uniformName << " for int arrays found in compiled shader after 2nd attempt!" << std::endl;
            }
        }

    }
    else
    {
        std::cout << "No uniform named "<< _uniformName << " for int arrays found in shader!" << std::endl;
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
        std::cout << "No uniform named "<< _uniformName << " found in shader!" << std::endl;
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
        std::cout << "No uniform named "<< _uniformName << " found in shader!" << std::endl;
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
        std::cout << "No uniform named "<< _uniformName << " found in shader!" << std::endl;
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
        std::cout << "No uniform named "<< _uniformName << " found in shader!" << std::endl;
    }
}

void Shader::setUniform(const ::std::string& _uniformName, const glm::mat4& _value)
{
    if(m_uniforms.m4Uniforms.find(_uniformName) != m_uniforms.m4Uniforms.end())
    {
        if(m_uniforms.m4Uniforms.at(_uniformName) != -1)
        {
            GLCall(glUniformMatrix4fv(m_uniforms.m4Uniforms[_uniformName], 1, GL_FALSE, &_value[0][0]));
        }
        else
        {
            GLint loc = getUniformLocation(_uniformName);
            if(loc == -1)
                std::cout << "No uniform var " << _uniformName << " for mat4 found in compiled shader!" << std::endl;
            else
            {
                std::cout << "Uniform var " << _uniformName << " for mat4 found in compiled shader after 2nd attempt!" << std::endl;
            }
        }
    }
    else
    {
        std::cout << "No uniform named "<< _uniformName << " found in shader!" << std::endl;
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
    //Int vectors
    for (auto& [varName, value] : m_uniforms.ivUniforms)
    {
        value.first = getUniformLocation(varName);
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
    //mat4
    for (auto& [varName, value] : m_uniforms.m4Uniforms)
    {
        value = getUniformLocation(varName);
    }
}
