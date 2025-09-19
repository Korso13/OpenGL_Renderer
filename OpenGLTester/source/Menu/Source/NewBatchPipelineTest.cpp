#include "../Public/NewBatchPipelineTest.h"

#include "BaseClasses/Nodes/Public/Camera.h"
#include "BaseClasses/Nodes/Public/Node.h"
#include "BaseClasses/RenderObjects/Public/Quad.h"
#include "BaseClasses/RenderObjects/Public/RenderPrimitive.h"
#include "ManagingClasses/Public/MeshesFactory.h"
#include "ManagingClasses/Public/Renderer.h"
#include "ManagingClasses/Public/MaterialLibrary.h"
#include "ManagingClasses/Public/ResourceLibrary.h"

NewBatchPipelineTest::NewBatchPipelineTest()
{
    m_root = M_SPTR<Node>("NewBatchPipelineTest_root");
    RENDERER->getRoot()->addChild(m_root);
    m_quad1 = MESH_FACTORY.buildQuad("logo", ShaderType::BATCH_RENDER, m_root, false, "logo1");
    m_quad2 = MESH_FACTORY.buildQuad("logo2", ShaderType::BATCH_RENDER, m_root, false, "logo2");
    
    RENDERER->getCamera()->setWorldPos(vec3(-500.f, -500.f, 0.f));
    // RENDERER->getCamera()->setProjectionMode(Camera::ProjectionMode::PERSPECTIVE);
    m_quad1->setWorldPos(vec3(0.f, 300.f, 0.f));
    m_quad1->setRenderOrder(1);
    m_quad2->setWorldPos(vec3( 300.f, 300.f, 0.f));
    m_quad2->setRenderOrder(1);
    
    std::vector<SPTR<Vertex>> vecticesForPrimitive;
    std::vector<size_t> indicesForPrimitive = {0,1,2};
    vecticesForPrimitive.emplace_back(CreateVertex(vec3(10.f, 0.f, 0.f), vec2(0.f, 0.f), 0, vec3(0.f), 0));
    vecticesForPrimitive.back()->color = vec4(1,0,0,1);
    vecticesForPrimitive.emplace_back(CreateVertex(vec3(250.f, 100.f, 0.f), vec2(1.f, 1.f), 1, vec3(0.f), 0));
    vecticesForPrimitive.back()->color = vec4(0,1,0,1);
    vecticesForPrimitive.emplace_back(CreateVertex(vec3(100.f, 0.f, 0.f), vec2(1.f, 0.f), 2, vec3(0.f), 0));
    vecticesForPrimitive.back()->color = vec4(0,0,1,1);
    m_testPrimitive = M_SPTR<RenderPrimitive>(
        "Test_Primitive",
        vecticesForPrimitive,
       std::move(indicesForPrimitive)
    );
    
    m_testPrimitive->setMatInst(MAT_LIB.getMaterial(ShaderType::BATCH_RENDER)->createInstance(), TextureId::TEX_0);
    m_testPrimitive->getMatInst()->setTexture(RL_TEXTURE("logo"));
    
    m_testPrimitive->setRenderOrder(1);
    m_root->addChild(m_testPrimitive);
    
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
