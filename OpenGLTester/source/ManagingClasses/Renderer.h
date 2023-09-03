#pragma once
#include "Defines.h"

class VertexAO;

class Renderer
{
    //defines constructor depending on what type of instantiation we want
#ifdef MULTIRENDER
public:
    Renderer() = default;
#endif
#ifdef SINGLERENDER
private:
    Renderer() = default;
    static Renderer* m_instance;

public:
    static Renderer* get()
    {
        if(!m_instance)
            m_instance = new Renderer();

        return m_instance;
    }
#endif

public:
    void draw(VertexAO* _vertexArray, const ShaderType _shaderType, GLint renderPrimitiveType = GL_TRIANGLES) const;
    void clear() const;
};
