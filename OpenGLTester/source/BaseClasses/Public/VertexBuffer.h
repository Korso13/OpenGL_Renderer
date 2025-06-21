#pragma once
#include <pch.h>
#include "../RenderObjects/Public/RenderObject.h"

class VertexBuffer
{
public:

    VertexBuffer(const void* _data, const unsigned int _size, GLenum _memUsageType = GL_STATIC_DRAW);
    VertexBuffer(const unsigned int _size = sizeof(Vertex)*1024);
    ~VertexBuffer();

    //Registers renderable object in the specific vertex buffer for batch-rendering
    //todo: redelegate to future RendererBatch class
    void addRenderObject(const std::string& _name,  SPTR<RenderObject> _objRef);
    void removeRenderObject(const std::string& _name);
    
    void bind() const;
    void unbind() const;
    
private:
    
    GLuint m_rendererId;
    std::unordered_map<std::string, SPTR<RenderObject>> m_renderPool;
    size_t m_VerticesToDraw = 0;
};
