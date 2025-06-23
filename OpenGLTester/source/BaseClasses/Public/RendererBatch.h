#pragma once
#include "Defines.h"

class VertexAO;
class RenderObject;

class RendererBatch
{
public:
    RendererBatch() = default;

    void addRenderObject(SPTR<RenderObject> _object);
    void removeRenderObject(uint32_t _objectUid);

private:

    std::unordered_map<uint32_t, WPTR<RenderObject>> m_batch;
    SPTR<VertexAO> m_vertexAOtoRender = nullptr;
    
};
