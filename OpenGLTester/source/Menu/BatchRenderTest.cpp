#include "BatchRenderTest.h"

#include "BaseClasses/IndexBuffer.h"
#include "BaseClasses/Texture.h"
#include "BaseClasses/VertexAttributes.h"
#include "BaseClasses/VertexBuffer.h"
#include "BaseClasses/RenderObjects/Quad.h"
#include "ManagingClasses/Renderer.h"
#include "ManagingClasses/ShaderMachine.h"
#include "ManagingClasses/VertexAO.h"
#include "Utilities/GLErrorCatcher.h"

BatchRenderTest::BatchRenderTest()
{
    prepareMVP();
    prepareTextures();

    //adding values to debug
    DEBUG_UI->addFolder("Batch Render Test");
    DEBUG_UI->addDebugValueToFolder<float>(
        "Batch Render Test",
        DebugDataType::FLOAT,
        "Camera position",
        &m_viewTranslation.x,
        2,
        ImGUI_ToolType::SLIDER,
        -1000.f,
        1000.f);
    DEBUG_UI->addDebugValueToFolder<float>(
        "Batch Render Test",
        DebugDataType::FLOAT,
        "Logo 1 position",
        &m_logo1Pos.x,
        2,
        ImGUI_ToolType::SLIDER,
        0.f,
        1000.f);
    DEBUG_UI->addDebugValueToFolder<float>(
        "Batch Render Test",
        DebugDataType::FLOAT,
        "Logo 2 position",
        &m_logo2Pos.x,
        2,
        ImGUI_ToolType::SLIDER,
        0.f,
        1000.f);
}

void BatchRenderTest::onUpdate(float dt)
{
}

void BatchRenderTest::onRender()
{
}

void BatchRenderTest::prepareMVP()
{
}

void BatchRenderTest::prepareTextures()
{
    m_vBuffer = M_SPTR<VertexBuffer>(*(new VertexBuffer));
    m_logoPlane1 = M_SPTR<Quad>(*(new Quad));
    m_logoPlane1->makeQuad({20,20}, m_logo1Pos, 0);
    m_logoPlane2 = M_SPTR<Quad>(*(new Quad));
    m_logoPlane1->makeQuad({30,30}, m_logo2Pos, 1);
    m_vBuffer->addRenderObject("Logo1", m_logoPlane1);
    m_vBuffer->addRenderObject("Logo2", m_logoPlane2);
    
    m_iBuffer = M_SPTR<IndexBuffer>(*(new IndexBuffer));
    m_iBuffer->push_back_drawIndices(m_logoPlane1);
    m_iBuffer->push_back_drawIndices(m_logoPlane2);

    m_vAttributes = M_SPTR<VertexAttributes>(*(new VertexAttributes));
    m_vAttributes->addAttribute({GL_FLOAT, 3, GL_FALSE});
    m_vAttributes->addAttribute({GL_FLOAT, 4, GL_FALSE});
    m_vAttributes->addAttribute({GL_FLOAT, 2, GL_FALSE});
    m_vAttributes->addAttribute({GL_INT, 1, GL_FALSE});
    m_vAttributes->addAttribute({GL_UNSIGNED_INT, 1, GL_FALSE});

    m_vao = M_SPTR<VertexAO>(*(new VertexAO()));
    //TODO:: transition all buffer wrappers to SPTR
    m_vao->addBuffer(m_vBuffer.get(), m_iBuffer.get(), m_vAttributes.get());

    //TODO:: rewrite to use new shader that takes batch-rendering attributes, multiple texture samplers
    //generating shader
    ShaderMachine::get()->setShader(ShaderType::TEXTURE_STD);
    m_texture1 = new Texture("resources/textures/logo.png");
    m_texture1->bind(0);
    ShaderMachine::get()->getShader(ShaderType::TEXTURE_STD)->setUniform("u_Texture", 0);
    ShaderMachine::get()->getShader(ShaderType::TEXTURE_STD)->setUniform("u_MVP", m_MVP);

    //setting blend functions
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    
    //Creating Renderer
    m_renderer = new Renderer;
}
