#include "../Public/RendererBatch.h"

#include "BaseClasses/Public/IndexBuffer.h"
#include "BaseClasses/Public/VertexAO.h"
#include "BaseClasses/Public/VertexBuffer.h"
#include "BaseClasses/RenderObjects/Public/RenderObject.h"
#include "Defines.h"
#include "BaseClasses/Nodes/Public/Camera.h"
#include "Utilities/Public/ImG_AutoDrawers.h"

RendererBatch::RendererBatch(const GLint _maxBatchSize)
    :
EngineInternal("RenderBatch"),
m_batchMaxSize(_maxBatchSize) 
{
    m_batch.reserve(_maxBatchSize);
    m_vertexBuffer = M_UPTR<VertexBuffer>();
    m_indexBuffer = M_UPTR<IndexBuffer>();
    m_vertexAOtoRender = M_UPTR<VertexAO>();
}

void RendererBatch::addRenderObject(const SPTR<RenderObject>& _object)
{
    if (!_object || isFull(static_cast<GLint>(_object->getMatInst()->getTexturesCount())))
        return;
    
    _object->m_isInBatch = true;
    _object->setIsDirty(false);
    m_batch[_object->getUID()] = _object;
    if (m_batchMaterial.expired())
    {
        m_batchMaterial = _object->getMatInst();
        m_heldTextures = static_cast<GLint>(_object->getMatInst()->getTexturesCount());
    }

    //recalculateBuffers(); //NOTE: less optimized than adding, but more readable and not prone to bugs (need to either modify VertexAO::addBufferTyped or reassemble it each time batch is changed to fix it!)
    m_vertexBuffer->addRenderObject(_object->getName(), _object);
    m_indexBuffer->addRenderObject(_object);
    m_vertexAOtoRender->clear(); //since we use batching, index and vertex buffers are common and we need to recreate VAO every time
    m_vertexAOtoRender->addBufferTyped<Vertex>(m_vertexBuffer, m_indexBuffer);
}

void RendererBatch::removeRenderObject(uint32_t _objectUid, bool _recalculateBuffers)
{
    if (m_batch.contains(_objectUid))
    {
        if(!m_batch[_objectUid].expired())
            m_batch[_objectUid].lock()->m_isInBatch = false;
        m_batch.erase(_objectUid);
    }
    else
        return;

    if(_recalculateBuffers)
        recalculateBuffers();
}

bool RendererBatch::isFull(const GLint _texturesForInsertion) const
{
    return m_heldTextures + _texturesForInsertion > m_batchMaxSize;
}

void RendererBatch::prepareForDraw() const
{
    ASSERT(!m_batchMaterial.expired()) //need the batch to have material

    //m_vertexBuffer->bind(); //todo: checking for possible fix
    m_batchMaterial.lock()->setMVP(RENDERER->getCamera()->getMvp()); 
    m_batchMaterial.lock()->inputUniformParamsToShader();

    //todo: checking for possible fix
    m_vertexAOtoRender->clear();
    m_vertexAOtoRender->addBufferTyped<Vertex>(m_vertexBuffer, m_indexBuffer);
}

ShaderType RendererBatch::getBatchShader() const
{
    if (!m_batchMaterial.expired())
        return m_batchMaterial.lock()->getShaderType();

    return ShaderType::NONE;
}

bool RendererBatch::onGui(const std::string& _name)
{
    bool result = false;
    result = result || AutoDrawers::DrawClassVariables("RendererBatch",
        NamedVar<const int*>{"Max batch size ", &m_batchMaxSize},
        NamedVar<int*>{"Held Textures", &m_heldTextures},
        NamedContainer<std::unordered_map<uint32_t, WPTR<RenderObject>>*>{"Batch contents", &m_batch},
        NamedVar<VertexBuffer*>{"Vertices", m_vertexBuffer.get()},
        NamedVar<IndexBuffer*>{"Indices", m_indexBuffer.get()},
        NamedVar<VertexAO*>{"Vertex AO", m_vertexAOtoRender.get()},
        NamedVar<MaterialInstance*>{"Material instance", m_batchMaterial.lock().get()},
        SubMenu{_name, SUB_MENU_CALL(return result = result || EngineInternal::onGui(_name);)}
    );
    if(result)
        recalculateBuffers();
    return result;
}

void RendererBatch::clearExpiredObjects()
{
    bool need_to_recalculate_buffers = false;
    std::set<uint32_t> deletionQueue;
    for(const auto& [uid, ObjectWeak] : m_batch)
    {
        if (ObjectWeak.expired() || ObjectWeak.lock()->isDirty())
        {
            need_to_recalculate_buffers = true;
            deletionQueue.insert(uid);
        }
    }
    for(const uint32_t& uid : deletionQueue)
        removeRenderObject(uid, false);
    if (need_to_recalculate_buffers) recalculateBuffers();
}

void RendererBatch::recalculateBuffers()
{
    //todo: needs thread safety
    m_vertexBuffer->clear();
    m_indexBuffer->clear();
    m_vertexAOtoRender->clear();
    if(!m_batchMaterial.expired())
        m_heldTextures = static_cast<GLint>(m_batchMaterial.lock()->getTexturesCount());
    
    for (const auto& [name, objectWptr] : m_batch)
    {
        if (objectWptr.expired())
            continue;

        auto object = objectWptr.lock();
        object->m_isInBatch = true;
        m_vertexBuffer->addRenderObject(object->getName(), object);
        m_indexBuffer->addRenderObject(object);
    }
    m_vertexAOtoRender->addBufferTyped<Vertex>(m_vertexBuffer, m_indexBuffer);
}
