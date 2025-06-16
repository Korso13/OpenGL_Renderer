#pragma once
#include <pch.h>
#include "Defines.h"

class RenderObject;
struct Vertex;

class IndexBuffer
{
public:

    IndexBuffer(const unsigned int* _data, const size_t _count, GLenum _memUsageType = GL_STATIC_DRAW);
    IndexBuffer(const size_t _count = 1024);
    ~IndexBuffer();

    //Primary method for adding indices to draw for batch-rendering method
    void addRenderObject(const SPTR<RenderObject>& _RenderObject);
    //Only use this one if you adjusted your indices elsewhere or it's the first batch of indices! Avoid if possible!
    void push_back_drawIndices(std::vector<size_t> _indices);

    void bind() const;
    void unbind() const;
    
    size_t getCount() const { return m_count; }

    //TODO: add methods for changing data, for "locking" buffer for data streaming
    
private:
    
    GLuint m_rendererId;
    size_t m_count;
    size_t m_adjustment = 0;

    std::vector<size_t> m_vertexIndicesPool; 
};
