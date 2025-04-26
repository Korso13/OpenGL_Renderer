#pragma once
#include <pch.h>
#include <Defines.h>
#include <fstream>

struct ShaderLib
{
public:
    typedef std::unordered_map<ShaderType, std::pair<const std::string,const std::string>> ShaderLibrary;

public:
    static ShaderLibrary& get()
    {
        if(m_shaderLibrary.empty())
           init();

        return m_shaderLibrary;
    }

private:
    
    static void init();
    
private:
    //key - shader enum, value - pair of vertex shader path and fragment shader path
    inline static ShaderLibrary m_shaderLibrary;
};