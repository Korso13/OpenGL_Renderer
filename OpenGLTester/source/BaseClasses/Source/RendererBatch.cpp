#include "../Public/RendererBatch.h"

#include "BaseClasses/Public/IndexBuffer.h"
#include "BaseClasses/Public/VertexAO.h"
#include "BaseClasses/Public/VertexBuffer.h"
#include "BaseClasses/RenderObjects/Public/RenderObject.h"

RendererBatch::RendererBatch(const GLint _maxBatchSize)
    : m_batchMaxSize(_maxBatchSize) 
{
    m_batch.reserve(_maxBatchSize);
    m_vertexBuffer = build::UnqPTR<VertexBuffer>();
    m_indexBuffer = build::UnqPTR<IndexBuffer>();
    m_vertexAOtoRender = build::UnqPTR<VertexAO>();
}

void RendererBatch::addRenderObject(const SPTR<RenderObject>& _object)
{
    if (!_object || isFull())
        return;
    
    _object->m_isInBatch = true;
    _object->setIsDirty(false);
    m_batch[_object->getUID()] = _object;
    if (m_batchMaterial.expired())
        m_batchMaterial = _object->getMatInst();

    m_vertexBuffer->addRenderObject(_object->getName(), _object);
    m_indexBuffer->addRenderObject(_object);
    
    //todo: add to buffers and etc
}

void RendererBatch::removeRenderObject(uint32_t _objectUid)
{
    if (m_batch.contains(_objectUid) && !m_batch[_objectUid].expired())
    {
        m_batch[_objectUid].lock()->m_isInBatch = false;
    }
    recalculateBuffers();
}

ShaderType RendererBatch::getBatchShader() const
{
    if (!m_batchMaterial.expired())
        return m_batchMaterial.lock()->getShaderType();

    return ShaderType::NONE;
}

void RendererBatch::clearExpiredObjects()
{
    std::erase_if(m_batch, [](const WPTR<RenderObject>& _object)
    {
        if (_object.expired())
            return true;
        if (_object.lock()->isDirty())
        {
            _object.lock()->m_isInBatch = false;
            return true;
        }
    });
}

void RendererBatch::recalculateBuffers()
{
    //todo: needs thread safety
    m_vertexBuffer->clear();
    m_indexBuffer->clear();
    m_vertexAOtoRender->clear();
    //todo: recalculate all buffers
}
