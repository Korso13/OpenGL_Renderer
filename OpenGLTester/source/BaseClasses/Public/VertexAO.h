#pragma once
#include "../../pch.h"

#include "Defines.h"
#include "BaseClasses/Public/IndexBuffer.h"
#include "BaseClasses/Public/VertexBuffer.h"

class VertexAttributes;

class VertexAO
{
public:

    VertexAO();
    ~VertexAO();

    //Specialized for certain Vertex format
    template<typename T>
    void addBufferTyped(const UPTR<VertexBuffer>& _vertexBuffer, const UPTR<IndexBuffer>& _indexBuffer);
    size_t getIndexCount() const {return m_indicesCount;}
    
    void bind() const;
    void unbind() const;
    void clear();

    bool onGui(const std::string& _name);
    
private:
    
    template<typename T>
    void setVertexAttributesTyped();

private:

    GLuint m_rendererId;
    size_t m_indicesCount = 0; //mostly for checks and debugging

    //todo: old impl, to be removed
    std::vector<VertexAttributes*> m_aBuffer;
};

//template functions implementations
#include "VertexAO.inl"