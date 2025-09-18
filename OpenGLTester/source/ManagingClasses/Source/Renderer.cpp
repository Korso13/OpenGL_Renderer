#include "../Public/Renderer.h"
#include "BaseClasses/Public/VertexAO.h"
#include "../Public/ShaderMachine.h"
#include "BaseClasses/Public/IndexBuffer.h"
#include "Utilities/Public/GLErrorCatcher.h"
#include "BaseClasses/Public/RendererBatch.h"
#include "../Public/ShaderMachine.h"
#include "BaseClasses/Nodes/Public/Node.h"
#include "BaseClasses/Nodes/Public/Camera.h"
#include "BaseClasses/RenderObjects/Public/RenderObject.h"
#include "Utilities/Public/ImG_AutoDrawers.h"

#ifdef SINGLERENDER
Renderer* Renderer::m_instance = nullptr;
#endif

Renderer::Renderer()
{
    GLCall(glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &m_maxBatchSize));
    if(m_maxBatchSize > MAX_RENDER_BATCH_SIZE)
    {
        std::cout << "[Init][Renderer] Batch size limited exceeded! Raise value for MAX_RENDER_BATCH_SIZE define! GPU limit: " << std::to_string(m_maxBatchSize) << std::endl;
        m_maxBatchSize = MAX_RENDER_BATCH_SIZE;
    }
    std::cout << "[Init][Renderer] Batch max size: " << std::to_string(m_maxBatchSize) << "\n";
    ShaderMachine::get();//forces shaders to precompile
    m_rootNode = M_SPTR<Node>("RenderRootNode");
    m_camera = SPTR<Camera>(new Camera); //not using M_SPTR due to private constructor limitation
}


void Renderer::render()
{
    //std::cout << "[Renderer::render]\n"; //todo: remove debug
    clear(); //clearing buffer

    //todo: remove after adding functionality to the MaterialInst
    //setting blend functions
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    
    updateRenderBatches();
    m_rootNode->traversal(std::bind(&Renderer::checkNodeForRender, this, std::placeholders::_1), true);

    //todo: add alternative, togglable batch assembly pipelines that ignore RenderOrders or z pos (2 std::function for checkNodeForRender and code below, that are switched on pipeline changes)
    //assembling m_sortedRenderPriority into ordered Renderer Batches
    for (auto&  [zDist, ro_container] : m_sortedRenderPriority)
    {
        for (auto& [RenderOrder, MIContainer] : ro_container)
        {
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

void Renderer::drawBatch(UPTR<RendererBatch>& _batch)
{
    _batch->prepareForDraw(); 
    if(!ShaderMachine::get()->setShader(_batch->getBatchShader()))
    {
        std::cout << "[Renderer::drawBatch] error - shader "<< std::to_string(CAST_I(_batch->getBatchShader()))<< " not found!\n";
        return;
    }

    ShaderMachine::get()->getShader(_batch->getBatchShader())->bind(); //todo: checking for possible fix
    
    const UPTR<VertexAO>& vao = _batch->getBatchVAO();
    vao->bind(); //binds vertex buffer, index buffer and vertex attributes
    const size_t index_size = (vao->getIndexCount() == 0) ? (6) : (vao->getIndexCount()); //precaution against vertexAO without index buffer
    GLCall(glDrawElements(GL_TRIANGLES, CAST_I(index_size), GL_UNSIGNED_INT, 0));
}

void Renderer::checkNodeForRender(const SPTR<Node>& _node)
{
    //std::cout << "[Renderer::checkNodeForRender]\n"; //todo: remove debug
    if(!_node || !_node->isEnabled() || !_node->isInFrustum())
        return;
    //std::cout << "[Renderer::checkNodeForRender]" << _node->getName() << "\n"; //todo: remove debug

    float distance_to_camera = 0.f;
    //todo: add distance to camera check!
    SPTR<RenderObject> renderableObject = CAST_SPTR<RenderObject>(_node);
    if(!renderableObject || !renderableObject->getMatInst() || renderableObject->isInBatch())
        return;
    
    //std::cout << "[Renderer::checkNodeForRender]" << _node->getName() << " valid renderableObject and renderableObject. Not in batch" << "\n"; //todo: remove debug

    //placing RenderObject to an existing Renderer batch or creating a new batch in automatically sorting multi-layered container, based on suggested rendering order
    //Priorities: distance to camera, set render order of the object + division into separate instances of MaterialInstance (shaders)
    auto& renderBatches =  m_sortedRenderPriority[distance_to_camera][renderableObject->getRenderOrder()][renderableObject->getMatInst()->getUID()];
    if (renderBatches.empty()) renderBatches.reserve(RESERVE_SAME_LEVEL_BATCHES);
    if (renderBatches.empty() || renderBatches.back()->isFull(static_cast<GLint>(renderableObject->getMatInst()->getTexturesCount()))) //no batches for this render layer or the last one is full (avoiding checking all batches)
    {
        renderBatches.emplace_back(M_UPTR<RendererBatch>(m_maxBatchSize));
    }
    renderBatches.back()->addRenderObject(renderableObject);
    //m_sortedRenderPriority[distance_to_camera][renderableObject->getRenderOrder()][renderableObject->getMatInst()->getUID()].push_back(renderableObject);
}

void Renderer::updateRenderBatches()
{
    //std::cout << "[Renderer::updateRenderBatches]\n"; //todo: remove debug
    for (auto&  [zDist, ro_container] : m_sortedRenderPriority)
    {
        for (auto& [RenderOrder, MIContainer] : ro_container)
        {
            for (auto& [MatInstUid, RenderObjContainer] : MIContainer)
            {
                std::erase_if(RenderObjContainer, [](const UPTR<RendererBatch>& _batch) 
                {
                    _batch->clearExpiredObjects(); //removes dirty and expired objects
                    return _batch->isExpired(); //removes empty batches
                });
            }
        }
    }
    //std::cout << "[Renderer::updateRenderBatches]Found valid batches " + STR(found) + "\n"; //todo: remove debug
}

bool Renderer::onGui(const std::string& _name)
{
    bool result = false;
    result = result || (AutoDrawers::AutoDraw("Max batch size", &m_maxBatchSize));
    
    //std::map<float, std::map<uint32_t, std::unordered_map<uint32_t, std::vector<UPTR<RendererBatch>>>>> m_sortedRenderPriority;
    size_t elements_batched = 0;
    const size_t z_layers_count = m_sortedRenderPriority.size();
    for (auto&  [zDist, ro_container] : m_sortedRenderPriority)
    {
        if (ImGui::TreeNode(("Distance: " + STR(zDist)).c_str()))
        {
            for (auto& [RenderOrder, MIContainer] : ro_container)
            {
                if (ImGui::TreeNode(("RO: " + STR(RenderOrder)).c_str()))
                {
                    for (auto& [MatInstUid, RenderObjContainer] : MIContainer)
                    {
                        result = result || (AutoDrawers::AutoDraw("MaterialInstId: " + STR(MatInstUid), &RenderObjContainer));
                        elements_batched += RenderObjContainer.size();
                    }
                    ImGui::TreePop();
                }
            }
            ImGui::TreePop();
        }
    }
    AutoDrawers::AutoDraw("Elements batched", &elements_batched);
    AutoDrawers::AutoDraw("Z layers: ", &z_layers_count);
    return result;
}
