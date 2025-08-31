#pragma once
#include <pch.h>
#include "Utilities/Public/Utilities.h"

class RenderObject;

class VertexBuffer : public EngineInternal
{
public:

    //VertexBuffer();
    VertexBuffer(const void* _data, const unsigned int _size, GLenum _memUsageType = GL_STATIC_DRAW);
    VertexBuffer(const unsigned int _size = sizeof(Vertex)*1024); //todo: probably should be templated vertex type + make Vertex interface class + requires std::is_base_v check 
    ~VertexBuffer() override;

    //Registers renderable object in the specific vertex buffer for batch-rendering
    void addRenderObject(const std::string& _name, const SPTR<RenderObject>& _objRef);
    void removeRenderObject(const std::string& _name);
    
    void updateVerticesPool(); //call this before binding to avoid vertices' caching issues (or add flags to determine whether you need it)
    void bind() const;
    void unbind() const;
    void clear();

    bool onGui(const std::string& _name) override;

private:
    
    GLuint m_rendererId;
    std::unordered_map<std::string, WPTR<RenderObject>> m_renderPool;
    std::vector<Vertex> m_verticesPool;
    size_t m_verticesToDraw = 0;
};
