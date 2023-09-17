#pragma once
#include <PCH.h>
#include "Defines.h"

class RenderObject;
struct Vertex;

class IndexBuffer
{
public:

    IndexBuffer(const unsigned int* _data, const unsigned int _count, GLenum _memUsageType = GL_STATIC_DRAW);
    IndexBuffer(const unsigned int _count = 100);
    ~IndexBuffer();

    //Primary method for adding indices to draw for batch-rendering method
    void push_back_drawIndices(const SPTR<RenderObject>& _RenderObject);
    //Only use this one if you adjusted your indices elsewhere or it's the first batch of indices! Avoid if possible!
    void push_back_drawIndices(std::vector<unsigned int> _indices);

    void bind() const;
    void unbind() const;
    
    unsigned int getCount() const { return m_count; }

    //TODO: add methods for changing data, for "locking" buffer for data streaming
    
private:
    
    GLuint m_rendererId;
    unsigned int m_count;

    std::vector<unsigned int> m_vertexIndicesPool; 
};
