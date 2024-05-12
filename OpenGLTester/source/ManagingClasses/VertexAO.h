#pragma once
#include <PCH.h>

#include "Defines.h"
#include "BaseClasses/IndexBuffer.h"
#include "BaseClasses/VertexBuffer.h"
#include "Utilities/GLErrorCatcher.h"

class VertexAttributes;

class VertexAO
{
public:

    VertexAO();
    ~VertexAO();

    void addBuffer(const VertexBuffer& _vertexBuffer, const VertexAttributes& _vertexAttrib);
    //More universal
    void addBuffer(VertexBuffer* _vertexBuffer, IndexBuffer* _indexBuffer, VertexAttributes* _vertexAttrib);

    //Specialized for certain Vertex format
    template<typename T>
    void addBufferTyped(VertexBuffer* _vertexBuffer, IndexBuffer* _indexBuffer);
    template<typename T>
    void setVertexAtributesTyped(){}
    
    void bind() const;
    void unbind() const;

    const std::vector<VertexBuffer*>& getVertexBuffers() const {return m_vBuffer;}
    const std::vector<IndexBuffer*>& getIndexBuffers() const {return m_iBuffer;}
    const std::vector<VertexAttributes*>& getAttributesBuffers() const {return m_aBuffer;}

//template functions implementations
#include "VertexAO.inl"
    
private:

    GLuint m_rendererId;
    std::vector<VertexBuffer*> m_vBuffer;
    std::vector<IndexBuffer*> m_iBuffer;
    std::vector<VertexAttributes*> m_aBuffer;
};
