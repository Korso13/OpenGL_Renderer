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

    //Specialized for certain Vertex format
    //TODO:: transition all buffer wrappers to SPTR
    template<typename T>
    void addBufferTyped(const UPTR<VertexBuffer>& _vertexBuffer, const UPTR<IndexBuffer>& _indexBuffer);
    template<typename T>
    void setVertexAttributesTyped();
    
    void bind() const;
    void unbind() const;
    void clear();

    //deprecated methods
    void addBuffer(const VertexBuffer& _vertexBuffer, const VertexAttributes& _vertexAttrib);
    //More universal
    void addBuffer(VertexBuffer* _vertexBuffer, IndexBuffer* _indexBuffer, VertexAttributes* _vertexAttrib);
    const std::vector<VertexBuffer*>& getVertexBuffers() const {return m_vBufferRaw;}
    const std::vector<IndexBuffer*>& getIndexBuffers() const {return m_iBufferRaw;}
    const std::vector<VertexAttributes*>& getAttributesBuffers() const {return m_aBuffer;}

//template functions implementations
#include "VertexAO.inl"
    
private:

    GLuint m_rendererId;
    /*std::vector<SPTR<VertexBuffer>> m_vBuffer;
    std::vector<SPTR<IndexBuffer>> m_iBuffer;*/

    //old impl
    std::vector<VertexBuffer*> m_vBufferRaw;
    std::vector<IndexBuffer*> m_iBufferRaw;
    std::vector<VertexAttributes*> m_aBuffer;
};
