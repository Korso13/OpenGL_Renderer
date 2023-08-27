#include "VertexAO.h"

#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexAttributes.h"
#include "Utilities/GLErrorCatcher.h"

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

void VertexAO::addBuffer(const VertexBuffer& _vertexBuffer, const IndexBuffer& _indexBuffer, const VertexAttributes& _vertexAttrib)
{
    //binding vertexArrayObject and buffers
    bind();
    _vertexBuffer.bind(); 
    _indexBuffer.bind();

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

void VertexAO::bind() const
{
    GLCall(glBindVertexArray(m_rendererId));
}

void VertexAO::unbind() const
{
    GLCall(glBindVertexArray(0));
}
    
