#include "../Public/NewBatchPipelineTest.h"

#include "BaseClasses/Nodes/Public/Camera.h"
#include "BaseClasses/Nodes/Public/Node.h"
#include "BaseClasses/RenderObjects/Public/Quad.h"
#include "ManagingClasses/Public/MeshesFactory.h"
#include "ManagingClasses/Public/Renderer.h"

NewBatchPipelineTest::NewBatchPipelineTest()
{
    m_root = M_SPTR<Node>("NewBatchPipelineTest_root");
    RENDERER->getRoot()->addChild(m_root);
    m_quad1 = MESH_FACTORY.buildQuad("logo", ShaderType::BATCH_RENDER, m_root, false, "logo1");
    m_quad2 = MESH_FACTORY.buildQuad("logo2", ShaderType::BATCH_RENDER, m_root, false, "logo2");

    RENDERER->getCamera()->setWorldPos(vec3(0.f, 0.f, -5.f));
    if (m_quad1) m_quad1->setWorldPos(vec3(-1.f, 0.f, 0.f));
    if (m_quad2) m_quad2->setWorldPos(vec3( 1.f, 0.f, 0.f));

    //adding values to debug
    DEBUG_UI->addFolder("New Batch Render Pipeline Test");
}

NewBatchPipelineTest::~NewBatchPipelineTest()
{
    
}

void NewBatchPipelineTest::onUpdate(float dt)
{
}

void NewBatchPipelineTest::onRender()
{
    RENDERER->render();
}
