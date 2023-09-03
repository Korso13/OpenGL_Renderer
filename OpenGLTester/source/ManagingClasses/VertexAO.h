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
    void addBuffer(VertexBuffer* _vertexBuffer, IndexBuffer* _indexBuffer, VertexAttributes* _vertexAttrib);
    
    void bind() const;
    void unbind() const;

    const std::vector<VertexBuffer*>& getVertexBuffers() const {return m_vBuffer;}
    const std::vector<IndexBuffer*>& getIndexBuffers() const {return m_iBuffer;}
    const std::vector<VertexAttributes*>& getAttributesBuffers() const {return m_aBuffer;}
    
private:

    GLuint m_rendererId;
    std::vector<VertexBuffer*> m_vBuffer;
    std::vector<IndexBuffer*> m_iBuffer;
    std::vector<VertexAttributes*> m_aBuffer;
};
