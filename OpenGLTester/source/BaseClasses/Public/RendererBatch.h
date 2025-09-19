#pragma once
#include "Defines.h"
#include "Utilities/Public/Utilities.h"

class MaterialInstance;
class IndexBuffer;
class VertexAO;
class VertexBuffer;
class RenderObject;

class RendererBatch : public EngineInternal
{
    friend class Renderer;
public:
    explicit RendererBatch(GLint _maxBatchSize);
    RendererBatch() = delete;
    RendererBatch(const RendererBatch&) = delete;
    ~RendererBatch() = default;

    void addRenderObject(const SPTR<RenderObject>& _object);
    void removeRenderObject(uint32_t _objectUid, bool _recalculateBuffers = true);

    bool isFull(GLint _texturesForInsertion) const;
    bool isExpired() const {return m_batch.empty();}

    // binds textures, sets MVP
    void prepareForDraw() const;
    
    UPTR<VertexAO>& getBatchVAO() { return m_vertexAOtoRender; }
    ShaderType getBatchShader() const;

    bool onGui(const std::string& _name) override;
    
private:

    void clearExpiredObjects();
    void recalculateBuffers();
    
private:

    const GLint m_batchMaxSize = 1; //set in constructor
    GLint m_heldTextures = 0; //batch max size is how many textures are we holding
    std::unordered_map<uint32_t, WPTR<RenderObject>> m_batch; //map of render objects. Key == object's UID
    UPTR<VertexBuffer> m_vertexBuffer = nullptr;
    UPTR<IndexBuffer> m_indexBuffer = nullptr;
    UPTR<VertexAO> m_vertexAOtoRender = nullptr;
    WPTR<MaterialInstance> m_batchMaterial; //all render objects in the batch have the same materialInstance
};
