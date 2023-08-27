#pragma once
#include <PCH.h>

class IndexBuffer
{
public:

    IndexBuffer(const unsigned int* _data, const unsigned int _count, GLenum _memUsageType = GL_STATIC_DRAW);
    ~IndexBuffer();

    void bind() const;
    void unbind() const;
    
    unsigned int getCount() const { return m_count; }

    //TODO: add methods for changing data, for "locking" buffer for data streaming
    
private:
    
    GLuint m_rendererId;
    unsigned int m_count;
};
