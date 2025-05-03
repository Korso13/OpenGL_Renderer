#include "../Public/Renderer.h"
#include "../Public/VertexAO.h"
#include "../Public/ShaderMachine.h"
#include "BaseClasses/Public/IndexBuffer.h"
#include "Utilities/Public/GLErrorCatcher.h"
#include "BaseClasses/Public/RendererBatch.h"
#include "../Public/ShaderMachine.h"
#include "BaseClasses/Nodes/Public/Node.h"

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
    std::cout << "[Init][Renderer] Batch max size: " << std::to_string(m_maxBatchSize) << std::endl;
    ShaderMachine::get();//forces shaders to precompile
    m_rootNode = M_SPTR<Node>("RenderRootNode");
}

void Renderer::draw(const VertexAO* _vertexArray, const ShaderType _shaderType, GLint _renderPrimitiveType) const
{
    if(!_vertexArray)
        return;
    
    if(!ShaderMachine::get()->setShader(_shaderType))
    {
        std::cout << "Renderer::draw error - shader "
                    << std::to_string(static_cast<int>(_shaderType))
                    << " not found!"
                    << std::endl;
        return;
    }
    _vertexArray->bind(); //binds vertex buffer, index buffer and vertex attributes
    const int indexSize = (_vertexArray->getIndexBuffers().empty()) ? (6) : (_vertexArray->getIndexBuffers()[0]->getCount()); //precaution against vertexAO without index buffer
    GLCall(glDrawElements(_renderPrimitiveType, indexSize, GL_UNSIGNED_INT, 0));
}

void Renderer::render()
{
    clear(); //clearing buffer
    m_sortedRenderPriority.clear();
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
                for (auto render_obj : RenderObjContainer)
                {
                    //todo: assemble RendererBatches based on RenderObj
                    
                }
            }
        }
    }
    
    //rendering collected batches and clearing queue
    for(const auto& batch : m_currentBatchQueue)
    {
        drawBatch(batch);
    }
    m_currentBatchQueue.clear();
    m_sortedRenderPriority.clear();
}

void Renderer::clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

SPTR<Node> Renderer::getRoot()
{
    return m_rootNode;
}

void Renderer::drawBatch(SPTR<RendererBatch> _batch)
{
    //todo: batch render logic
}

void Renderer::checkNodeForRender(SPTR<Node> _node)
{
    if(!_node || !_node->isEnabled() || !_node->isInFrustum())
        return;

    float distance_to_camera = 0.f;
    //todo: add distance to camera check!
    SPTR<RenderObject> renderableObject = CAST_SPTR<RenderObject>(_node);
    if(!renderableObject || !renderableObject->getMatInst())
        return;

    //placing RenderObject to automatically sorting multi-layered container, based on suggested rendering order
    //Priorities: distance to camera, set render order of the object + division into separate instances of MaterialInstance (shaders)
    m_sortedRenderPriority[distance_to_camera][renderableObject->getRenderOrder()][renderableObject->getMatInst()->getUID()].push_back(renderableObject);
}
