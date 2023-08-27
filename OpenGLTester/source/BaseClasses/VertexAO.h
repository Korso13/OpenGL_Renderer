#pragma once
#include <PCH.h>

class IndexBuffer;
class VertexAttributes;
class VertexBuffer;

class VertexAO
{
public:

    VertexAO();
    ~VertexAO();

    void addBuffer(const VertexBuffer& _vertexBuffer, const VertexAttributes& _vertexAttrib);
    void addBuffer(const VertexBuffer& _vertexBuffer, const IndexBuffer& _indexBuffer, const VertexAttributes& _vertexAttrib);

    void bind() const;
    void unbind() const;
    
private:

    GLuint m_rendererId;
    
};
