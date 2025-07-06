#pragma once
#include "Utilities/Public/GLErrorCatcher.h"

//can be later rewritten via BOOST macro as an inline template function (not method) + Vertex struct declaration
template <>
inline void VertexAO::setVertexAttributesTyped<Vertex>()
{
    //vec3 position;
    GLCall(glEnableVertexAttribArray(0)); //setting new vertex attributes array
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, vCoord)));
    //vec3 position;
    GLCall(glEnableVertexAttribArray(1)); //setting new vertex attributes array
    GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, position)));
    //vec4 color; 
    GLCall(glEnableVertexAttribArray(2)); //setting new vertex attributes array
    GLCall(glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, color)));
    //vec2 UV;
    GLCall(glEnableVertexAttribArray(3)); //setting new vertex attributes array
    GLCall(glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, uv)));
    //int textureId;
    GLCall(glEnableVertexAttribArray(4)); //setting new vertex attributes array
    GLCall(glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, textureId)));
    //unsigned int vertexIndex;
    GLCall(glEnableVertexAttribArray(5)); //setting new vertex attributes array
    GLCall(glVertexAttribPointer(5, 1, GL_UNSIGNED_INT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, vertexIndex)));
}

template<>
inline void VertexAO::addBufferTyped<Vertex>(const UPTR<VertexBuffer>& _vertexBuffer, const UPTR<IndexBuffer>& _indexBuffer)
{
    //binding vertexArrayObject and buffers
    bind();
    _vertexBuffer->bind(); 
    _indexBuffer->bind();
    m_indicesCount += _indexBuffer->getCount();

    //setting vertex attribute array
    setVertexAttributesTyped<Vertex>();
}