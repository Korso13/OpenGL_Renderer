#include "../Public/BatchRenderTest.h"

#include "BaseClasses/Public/IndexBuffer.h"
#include "BaseClasses/Public/Texture.h"
#include "BaseClasses/Public/VertexAttributes.h"
#include "BaseClasses/Public/VertexBuffer.h"
#include "BaseClasses/RenderObjects/Public/Quad.h"
#include "ManagingClasses/Public/Renderer.h"
#include "ManagingClasses/Public/ShaderMachine.h"
#include "BaseClasses/Public/VertexAO.h"
#include "Utilities/Public/GLErrorCatcher.h"

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
    DEBUG_UI->addDebugButtonToFolder(
        "Batch Render Test",
        "Recache shader",
        [this]()
        {
            auto shader =  ShaderMachine::get()->getShader(ShaderType::BATCH_RENDER);
            if(shader) shader->debugRecacheUniforms();
        });
    DEBUG_UI->addDebugValueToFolder<int>(
        "Batch Render Test",
        DebugDataType::INT,
        "u_debugTexIndex value (texIndex below triggers coloring)",
        &m_debugTexIndex,
        1,
        ImGUI_ToolType::SLIDER,
        -10,
        20);
}

void BatchRenderTest::onUpdate(float dt)
{
    
}

void BatchRenderTest::onRender()
{
    if(!m_renderer)
        return;

    //setting logos' transforms
    updateQuadsTranslations();
    m_vBuffer->bind(); //memo: bugs out the orange field
    
    GLCall(glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a));
    m_renderer->clear();

    //calculating MVP
    m_view = glm::translate(glm::mat4(1.f), m_viewTranslation);
    const auto model = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, 0.f));
    m_MVP =  m_projection * m_view * model;
    auto shader = ShaderMachine::get()->getShader(ShaderType::BATCH_RENDER);
    if(!shader)
    {
        std::cout << "Shader BATCH_RENDER not found!\n";
        return;
    }
    shader->setUniform("u_MVP", m_MVP);

    //making a single draw call
    if(m_texture1) m_texture1->bind(0); 
    if(m_texture2) m_texture2->bind(1);
    m_renderer->draw(m_vao.get(), ShaderType::BATCH_RENDER);
}

void BatchRenderTest::prepareMVP()
{
    //making starting MVP matrix
    m_view = glm::translate(glm::mat4(1.f), m_viewTranslation); //Camera projection
    auto model = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, 0.f)); //rendered primitives position
    m_MVP =  m_projection * m_view * model;
}

void BatchRenderTest::prepareTextures()
{
    m_texture1 = new Texture("resources/textures/logo.png");
    m_texture2 = new Texture("resources/textures/logo2.png");
    if(m_texture2)
        m_texture2->bind(0);
    m_texture1->bind(1);
    int textures[2] = {0, 1};
    
    m_vBuffer = M_SPTR<VertexBuffer>(*(new VertexBuffer));
    m_iBuffer = M_SPTR<IndexBuffer>(*(new IndexBuffer));

    m_logoPlane1 = Quad::build(uvec2(300,300), m_logo1Pos, 0);
    m_vBuffer->addRenderObject("Logo1", m_logoPlane1);
    m_iBuffer->addRenderObject(m_logoPlane1);
    
    m_logoPlane2 = Quad::build(uvec2(300,300), m_logo2Pos, 1);
    m_vBuffer->addRenderObject("Logo2", m_logoPlane2);
    m_iBuffer->addRenderObject(m_logoPlane2);

    m_vao = M_SPTR<VertexAO>(*(new VertexAO()));
    m_vao->addBufferTyped<Vertex>(m_vBuffer.get(), m_iBuffer.get());
    
    //generating shader
    if(!ShaderMachine::get()->setShader(ShaderType::BATCH_RENDER))
    {
        std::cout << "Setting shader BATCH_RENDER failed, shader not found!" << std::endl;
        return;
    }

    auto shader =  ShaderMachine::get()->getShader(ShaderType::BATCH_RENDER);
    if(!shader)
    {
        std::cout << "Shader BATCH_RENDER not found!" << std::endl;
        return;
    }
    shader->bind();
    shader->setUniform("u_Textures", textures, 2);
    shader->setUniform("u_MVP", m_MVP);
    
    //setting blend functions
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    
    //Creating Renderer
    m_renderer = Renderer::get();
}

void BatchRenderTest::updateQuadsTranslations()
{
    if(m_logoPlane1)m_logoPlane1->setWorldPos(m_logo1Pos);
    if(m_logoPlane2)m_logoPlane2->setWorldPos(m_logo2Pos);
}
