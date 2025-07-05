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
    if (!_object || isFull(static_cast<GLint>(_object->getMatInst()->getTexturesCount())))
        return;
    
    _object->m_isInBatch = true;
    _object->setIsDirty(false);
    m_batch[_object->getUID()] = _object;
    if (m_batchMaterial.expired())
        m_batchMaterial = _object->getMatInst();
    
    m_heldTextures += static_cast<GLint>(_object->getMatInst()->getTexturesCount());
    m_vertexBuffer->addRenderObject(_object->getName(), _object);
    m_indexBuffer->addRenderObject(_object);
    m_vertexAOtoRender->addBufferTyped<Vertex>(m_vertexBuffer, m_indexBuffer);
}

void RendererBatch::removeRenderObject(uint32_t _objectUid)
{
    if (m_batch.contains(_objectUid) && !m_batch[_objectUid].expired())
    {
        m_batch[_objectUid].lock()->m_isInBatch = false;
    }
    recalculateBuffers();
}

bool RendererBatch::isFull(const GLint _texturesForInsertion) const
{
    return m_heldTextures + _texturesForInsertion > m_batchMaxSize;
}

ShaderType RendererBatch::getBatchShader() const
{
    if (!m_batchMaterial.expired())
        return m_batchMaterial.lock()->getShaderType();

    return ShaderType::NONE;
}

void RendererBatch::clearExpiredObjects()
{
    bool need_to_recalculate_buffers = false;
    std::erase_if(m_batch, [&need_to_recalculate_buffers](const WPTR<RenderObject>& _object)
    {
        if (_object.expired())
        {
            need_to_recalculate_buffers = true;
            return true;
        }
        if (_object.lock()->isDirty())
        {
            _object.lock()->m_isInBatch = false;
            need_to_recalculate_buffers = true;
            return true;
        }
        
        return false;
    });

    if (need_to_recalculate_buffers) recalculateBuffers();
}

void RendererBatch::recalculateBuffers()
{
    //todo: needs thread safety
    m_vertexBuffer->clear();
    m_indexBuffer->clear();
    m_vertexAOtoRender->clear();
    m_heldTextures = 0;
    
    for (const auto& [name, objectWptr] : m_batch)
    {
        if (objectWptr.expired())
            continue;

        auto object = objectWptr.lock();
        object->m_isInBatch = true;
        m_heldTextures += static_cast<GLint>(object->getMatInst()->getTexturesCount());
        m_vertexBuffer->addRenderObject(object->getName(), object);
        m_indexBuffer->addRenderObject(object);
        m_vertexAOtoRender->addBufferTyped<Vertex>(m_vertexBuffer, m_indexBuffer);
    }
}
