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
        -300.f,
        300.f);
    DEBUG_UI->addDebugValueToFolder<float>(
        "Batch Render Test",
        DebugDataType::FLOAT,
        "Logo 1 position",
        &m_logo1Pos.x,
        2,
        ImGUI_ToolType::SLIDER,
        0.f,
        600.f);
    DEBUG_UI->addDebugValueToFolder<float>(
        "Batch Render Test",
        DebugDataType::FLOAT,
        "Logo 2 position",
        &m_logo2Pos.x,
        2,
        ImGUI_ToolType::SLIDER,
        0.f,
        600.f);
}

void BatchRenderTest::onUpdate(float dt)
{
    
}

void BatchRenderTest::onRender()
{
    if(!m_renderer)
        return;

    GLCall(glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a));
    m_renderer->clear();

    //setting logos' transforms
    updateQuadsTranslations();

    //calculating MVP
    m_view = glm::translate(glm::mat4(1.f), m_viewTranslation);
    m_MVP =  m_projection * m_view * glm::mat4(1.f);
    auto shader = ShaderMachine::get()->getShader(ShaderType::BATCH_RENDER);
    if(!shader)
    {
        std::cout << "Shader BATCH_RENDER not found!" << std::endl;
        return;
    }
    shader->setUniform("u_MVP", m_MVP);

    //making a single draw call
    m_texture1->bind(0); 
    m_texture2->bind(1); 
    m_renderer->draw(m_vao.get(), ShaderType::BATCH_RENDER);
}

void BatchRenderTest::prepareMVP()
{
    //making starting MVP matrix
    m_view = glm::translate(glm::mat4(1.f), m_viewTranslation);
    m_MVP =  m_projection * m_view * glm::mat4(1.f);
}

void BatchRenderTest::prepareTextures()
{
    m_vBuffer = M_SPTR<VertexBuffer>(*(new VertexBuffer));
    m_logoPlane1 = M_SPTR<Quad>(*(new Quad));
    m_logoPlane1->makeQuad({200,200}, m_logo1Pos, 0);
    m_logoPlane2 = M_SPTR<Quad>(*(new Quad));
    m_logoPlane1->makeQuad({300,300}, m_logo2Pos, 1);
    m_vBuffer->addRenderObject("Logo1", m_logoPlane1);
    m_vBuffer->addRenderObject("Logo2", m_logoPlane2);
    
    m_iBuffer = M_SPTR<IndexBuffer>(*(new IndexBuffer));
    m_iBuffer->push_back_drawIndices(m_logoPlane1);
    m_iBuffer->push_back_drawIndices(m_logoPlane2);

    m_vAttributes = M_SPTR<VertexAttributes>(*(new VertexAttributes));
    m_vAttributes->addAttribute({GL_FLOAT, 3, GL_FALSE});
    m_vAttributes->addAttribute({GL_FLOAT, 4, GL_FALSE});
    m_vAttributes->addAttribute({GL_FLOAT, 2, GL_FALSE});
    m_vAttributes->addAttribute({GL_FLOAT, 1, GL_FALSE});
    m_vAttributes->addAttribute({GL_UNSIGNED_INT, 1, GL_FALSE});

    m_vao = M_SPTR<VertexAO>(*(new VertexAO()));
    //TODO:: transition all buffer wrappers to SPTR
    m_vao->addBuffer(m_vBuffer.get(), m_iBuffer.get(), m_vAttributes.get());
    
    //generating shader
    if(!ShaderMachine::get()->setShader(ShaderType::BATCH_RENDER))
    {
        std::cout << "Setting shader BATCH_RENDER failed, shader not found!" << std::endl;
        return;
    }
    m_texture1 = new Texture("resources/textures/logo.png");
    m_texture2 = new Texture("resources/textures/logo2.png");
    m_texture1->bind(0);
    m_texture2->bind(1);
    int* textures = new int[2];
    textures[0] = 0;
    textures[1] = 1;
    auto shader =  ShaderMachine::get()->getShader(ShaderType::BATCH_RENDER);
    if(!shader)
    {
        std::cout << "Shader BATCH_RENDER not found!" << std::endl;
        return;
    }
    shader->Bind();
    shader->setUniform("u_Texture", textures, 2);
    //shader->setUniform("u_TextureFallback", 0);
    //shader->setUniform("u_Texture1", 1);
    shader->setUniform("u_MVP", m_MVP);

    //setting blend functions
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    
    //Creating Renderer
    m_renderer = new Renderer;
}

void BatchRenderTest::updateQuadsTranslations()
{
    m_logoPlane1->setPosition(m_logo1Pos);
    m_logoPlane2->setPosition(m_logo2Pos);
}
