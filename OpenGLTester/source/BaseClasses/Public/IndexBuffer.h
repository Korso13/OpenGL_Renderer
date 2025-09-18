#pragma once
#include <pch.h>
#include "Defines.h"
#include "Utilities/Public/Utilities.h"

class RenderObject;
struct Vertex;

class IndexBuffer : public EngineInternal
{
public:

    IndexBuffer(const unsigned int* _data, const size_t _count, GLenum _memUsageType = GL_STATIC_DRAW);
    IndexBuffer(const size_t _count = 1024); //todo: make 1024 a global constexpr or a macro + add it to VertexBuffer
    ~IndexBuffer() override;

    //Primary method for adding indices to draw for batch-rendering method
    void addRenderObject(const SPTR<RenderObject>& _renderObject);

    void bind() const;
    void unbind() const;
    void clear();
    size_t getCount() const { return m_count; }

    bool onGui(const std::string& _name) override;

    //TODO: add methods for changing data, for "locking" buffer for data streaming
    
private:
    
    GLuint m_rendererId;
    size_t m_count;
    size_t m_adjustment = 0;

    std::vector<size_t> m_vertexIndicesPool; 
};
