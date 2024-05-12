#pragma once
#include <PCH.h>
#include <Defines.h>
#include "GLM/glm.hpp"

//shader program wrapper class
class Shader
{
public:

    Shader(const std::string& _vsPath, const std::string& _fsPath);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    //Returns the list of shader's uniform variables. Can't be modified outside setter methods 
    const Uniforms& getAllUniforms() const {return m_uniforms;}
    
    void setUniform(const std::string& _uniformName, int _value);
    void setUniform(const std::string& _uniformName, bool _value);
    void setUniform(const std::string& _uniformName, int* _value, int count);
    void setUniform(const std::string& _uniformName, float _value);
    void setUniform(const std::string& _uniformName, vec2 _value);
    void setUniform(const std::string& _uniformName, vec3 _value);
    void setUniform(const std::string& _uniformName, vec4 _value);
    void setUniform(const std::string& _uniformName, const glm::mat4& _value);

    void debugRecacheUniforms() {cacheUniformLocations();}
    
private:
    
    //service function for getting uniform variable location in compiled shader
    GLint getUniformLocation(const std::string& name);

    //caches uniform locations for further use
    void cacheUniformLocations();
    
private:

    GLuint m_RendererID;
    
    //holds uniform names and locations
    Uniforms m_uniforms;
};
