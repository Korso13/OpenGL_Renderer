#pragma once
#include "Defines.h"

class MaterialInstance;
class IndexBuffer;
class VertexAO;
class VertexBuffer;
class RenderObject;

class RendererBatch
{
    friend class Renderer;
public:
    RendererBatch(GLint _maxBatchSize);
    RendererBatch() = delete;
    RendererBatch(const RendererBatch&) = delete;
    ~RendererBatch() = default;

    void addRenderObject(const SPTR<RenderObject>& _object);
    void removeRenderObject(uint32_t _objectUid);

    bool isFull() const {return CAST_I(m_batch.size()) == m_batchMaxSize;} 
    bool isExpired() const {return m_batch.empty();}

    //set u_MVP, bind textures
    void prepareForDraw();
    
    UPTR<VertexAO>& getBatchVAO() { return m_vertexAOtoRender; }
    ShaderType getBatchShader() const;
    
private:

    void clearExpiredObjects();
    void recalculateBuffers();
    
private:

    const GLint m_batchMaxSize = 1; //set in constructor
    std::unordered_map<uint32_t, WPTR<RenderObject>> m_batch; //map of render objects. Key == object's UID
    UPTR<VertexBuffer> m_vertexBuffer = nullptr;
    UPTR<IndexBuffer> m_indexBuffer = nullptr;
    UPTR<VertexAO> m_vertexAOtoRender = nullptr;
    WPTR<MaterialInstance> m_batchMaterial; //all render objects in the batch have the same materialInstance
};
