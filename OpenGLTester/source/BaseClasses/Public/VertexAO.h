#pragma once
#include "../../pch.h"

#include "Defines.h"
#include "BaseClasses/Public/IndexBuffer.h"
#include "BaseClasses/Public/VertexBuffer.h"
#include "Utilities/Public/GLErrorCatcher.h"

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
    //TODO:: transition all buffer wrappers to SPTR
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
    //TODO:: transition all buffer wrappers to SPTR
    std::vector<VertexBuffer*> m_vBuffer;
    std::vector<IndexBuffer*> m_iBuffer;
    std::vector<VertexAttributes*> m_aBuffer;
};
