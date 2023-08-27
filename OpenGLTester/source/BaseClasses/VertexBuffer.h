#pragma once
#include <PCH.h>

class VertexBuffer
{
public:

    VertexBuffer(const void* _data, const unsigned int _size, GLenum _memUsageType = GL_STATIC_DRAW);
    ~VertexBuffer();

    void bind() const;
    void unbind() const;

    //TODO: add methods for changing data, for "locking" buffer for data streaming
    
private:
    
    GLuint m_rendererId;
    
};
