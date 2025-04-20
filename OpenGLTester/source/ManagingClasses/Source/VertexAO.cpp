#include "../Public/VertexAO.h"

#include "BaseClasses/Public/IndexBuffer.h"
#include "BaseClasses/Public/VertexBuffer.h"
#include "BaseClasses/Public/VertexAttributes.h"
#include "Utilities/Public/GLErrorCatcher.h"

VertexAO::VertexAO()
{
    GLCall(glGenVertexArrays(1, &m_rendererId));
}

VertexAO::~VertexAO()
{
    GLCall(glDeleteVertexArrays(1, &m_rendererId));
}

void VertexAO::addBuffer(const VertexBuffer& _vertexBuffer, const VertexAttributes& _vertexAttrib)
{
    //binding vertexArrayObject and vertex buffer
    bind();
    _vertexBuffer.bind();

    //setting vertex attribute array
    const auto& attributes = _vertexAttrib.getVertexAttributes();
    unsigned int offset = 0;
    for (unsigned int i = 0; i < attributes.size(); i++)
    {
        GLCall(glEnableVertexAttribArray(i)); //setting new vertex attributes array
        GLCall(glVertexAttribPointer(i, attributes[i].count, attributes[i].type, attributes[i].isNotNormalized, _vertexAttrib.getStride(), (const void*)offset));

        offset += attributes[i].count * _vertexAttrib.getTypeSize(attributes[i].type);
    }
}

void VertexAO::addBuffer(VertexBuffer* _vertexBuffer, IndexBuffer* _indexBuffer, VertexAttributes* _vertexAttrib)
{
    m_vBuffer.push_back(_vertexBuffer);
    m_iBuffer.push_back(_indexBuffer);
    m_aBuffer.push_back(_vertexAttrib);
    
    //binding vertexArrayObject and buffers
    bind();
    _vertexBuffer->bind(); 
    _indexBuffer->bind();

    //setting vertex attribute array
    setVertexAtributesTyped<Vertex>();
}

void VertexAO::bind() const
{
    GLCall(glBindVertexArray(m_rendererId));
}

void VertexAO::unbind() const
{
    GLCall(glBindVertexArray(0));
}
    
