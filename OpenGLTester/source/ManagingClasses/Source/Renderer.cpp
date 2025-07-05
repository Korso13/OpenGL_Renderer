#include "../Public/Renderer.h"
#include "BaseClasses/Public/VertexAO.h"
#include "../Public/ShaderMachine.h"
#include "BaseClasses/Public/IndexBuffer.h"
#include "Utilities/Public/GLErrorCatcher.h"
#include "BaseClasses/Public/RendererBatch.h"
#include "../Public/ShaderMachine.h"
#include "BaseClasses/Nodes/Public/Node.h"
#include "BaseClasses/Nodes/Public/Camera.h"

#ifdef SINGLERENDER
Renderer* Renderer::m_instance = nullptr;
#endif

Renderer::Renderer()
{
    GLCall(glGetIntegerv(GL_MAX_TEXTURE_SIZE, &m_maxBatchSize));
    if(m_maxBatchSize > MAX_RENDER_BATCH_SIZE)
    {
        std::cout << "[Init][Renderer] Batch size limited exceeded! Raise value for MAX_RENDER_BATCH_SIZE define! GPU limit: " << std::to_string(m_maxBatchSize) << std::endl;
        m_maxBatchSize = MAX_RENDER_BATCH_SIZE;
    }
    std::cout << "[Init][Renderer] Batch max size: " << std::to_string(m_maxBatchSize) << "\n";
    ShaderMachine::get();//forces shaders to precompile
    m_rootNode = M_SPTR<Node>("RenderRootNode");
    m_camera = std::shared_ptr<Camera>(new Camera); //alternative SPTR creation due to private constructor
}


void Renderer::render()
{
    clear(); //clearing buffer
    updateRenderBatches();
    m_rootNode->traversal(std::bind(&Renderer::checkNodeForRender, this, std::placeholders::_1), true);

    //todo: add alternative, togglable batch assembly pipelines that ignore RenderOrders or z pos (2 std::function for checkNodeForRender and code below, that are switched on pipeline changes)
    //assembling m_sortedRenderPriority into ordered Renderer Batches
    for (auto TopLevelIt = m_sortedRenderPriority.rbegin(); TopLevelIt != m_sortedRenderPriority.rend(); TopLevelIt++)
    {
        auto roContainer =  (*TopLevelIt).second;
        for (auto MidLevelIt = roContainer.begin(); MidLevelIt != roContainer.end(); MidLevelIt++)
        {
            auto MIContainer =  (*MidLevelIt).second;
            for (auto& [MatInstUid, RenderObjContainer] : MIContainer)
            {
                for (auto& render_batch : RenderObjContainer)
                {
                    drawBatch(render_batch);
                }
            }
        }
    }
}

void Renderer::clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

SPTR<Node> Renderer::getRoot()
{
    return m_rootNode;
}

SPTR<Camera> Renderer::getCamera()
{
    return m_camera;
}

void Renderer::drawBatch(const UPTR<RendererBatch>& _batch)
{
    UPTR<VertexAO>& vao = _batch->getBatchVAO();
    if(!ShaderMachine::get()->setShader(_batch->getBatchShader()))
    {
        std::cout << "Renderer::draw error - shader "
                    << std::to_string(static_cast<int>(_batch->getBatchShader()))
                    << " not found!\n";
        return;
    }
    //shader->setUniform("u_MVP", m_MVP); //can technically set mvp here instead of prepareForDraw();
    _batch->prepareForDraw(); 
    vao->bind(); //binds vertex buffer, index buffer and vertex attributes
    const size_t indexSize = (vao->getIndexBuffers().empty()) ? (6) : (vao->getIndexBuffers()[0]->getCount()); //precaution against vertexAO without index buffer
    GLCall(glDrawElements(GL_TRIANGLES, CAST_I(indexSize), GL_UNSIGNED_INT, 0));
}

void Renderer::checkNodeForRender(const SPTR<Node>& _node)
{
    if(!_node || !_node->isEnabled() || !_node->isInFrustum())
        return;

    float distance_to_camera = 0.f;
    //todo: add distance to camera check!
    SPTR<RenderObject> renderableObject = CAST_SPTR<RenderObject>(_node);
    if(!renderableObject || !renderableObject->getMatInst() || renderableObject->isInBatch())
        return;

    //placing RenderObject to an existing Renderer batch or creating a new batch in automatically sorting multi-layered container, based on suggested rendering order
    //Priorities: distance to camera, set render order of the object + division into separate instances of MaterialInstance (shaders)
    auto& renderBatches =  m_sortedRenderPriority[distance_to_camera][renderableObject->getRenderOrder()][renderableObject->getMatInst()->getUID()];
    if (renderBatches.empty() || renderBatches.back()->isFull(static_cast<GLint>(renderableObject->getMatInst()->getTexturesCount()))) //no batches for this render layer or the last one is full (avoiding checking all batches)
    {
        renderBatches.emplace_back(build::UnqPTR<RendererBatch>(m_maxBatchSize));
    }
    renderBatches.back()->addRenderObject(renderableObject);
    //m_sortedRenderPriority[distance_to_camera][renderableObject->getRenderOrder()][renderableObject->getMatInst()->getUID()].push_back(renderableObject);
}

void Renderer::updateRenderBatches()
{
    for (auto TopLevelIt = m_sortedRenderPriority.rbegin(); TopLevelIt != m_sortedRenderPriority.rend(); TopLevelIt++)
    {
        auto roContainer =  (*TopLevelIt).second;
        for (auto MidLevelIt = roContainer.begin(); MidLevelIt != roContainer.end(); MidLevelIt++)
        {
            auto MIContainer =  (*MidLevelIt).second;
            for (auto& [MatInstUid, RenderObjContainer] : MIContainer)
            {
                std::erase_if(RenderObjContainer, [](const SPTR<RendererBatch>& batch) 
                {
                    batch->clearExpiredObjects(); //removes dirty and expired objects
                    return batch->isExpired(); //removes empty batches
                });
            }
        }
    }
}
