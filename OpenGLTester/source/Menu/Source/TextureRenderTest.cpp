#include "../Public/TextureRenderTest.h"
#include "Defines.h"
#include "BaseClasses/Public/Texture.h"
#include "ManagingClasses/Public/Renderer.h"
#include "ManagingClasses/Public/ShaderMachine.h"
#include "BaseClasses/Public/IndexBuffer.h"
#include "BaseClasses/Public/VertexBuffer.h"
#include "BaseClasses/Public/VertexAO.h"
#include "BaseClasses/Public/VertexAttributes.h"
#include "Utilities/Public/GLErrorCatcher.h"

TextureRenderTest::TextureRenderTest()
{
    prepareMVP();
    prepareTextures();

    DEBUG_UI->addFolder("Texture Render Test");
    //adding values to debug
    DEBUG_UI->addDebugValueToFolder<bool>(
        "Texture Render Test",
        DebugDataType::BOOL,
        "Show 2nd logo",
        &m_show2NdLogo,
        1,
        ImGUI_ToolType::CHECKBOX);

    DEBUG_UI->addDebugValueToFolder<float>(
        "Texture Render Test",
        DebugDataType::FLOAT,
        "Camera position",
        &m_viewTranslation.x,
        2,
        ImGUI_ToolType::SLIDER,
        -300.f,
        300.f);
    DEBUG_UI->addDebugValueToFolder<float>(
        "Texture Render Test",
        DebugDataType::FLOAT,
        "Logo 1 position",
        &m_logo1Pos.x,
        2,
        ImGUI_ToolType::SLIDER,
        0.f,
        500.f);
    DEBUG_UI->addDebugValueToFolder<float>(
        "Texture Render Test",
        DebugDataType::FLOAT,
        "Logo 2 position",
        &m_logo2Pos.x,
        2,
        ImGUI_ToolType::SLIDER,
        0.f,
        500.f);
}

TextureRenderTest::~TextureRenderTest()
{
    delete m_vao1;
    delete m_vAttributes1;
    delete m_iBuffer1;
    delete m_vBuffer1;
    delete m_texture1;
}

void TextureRenderTest::onUpdate(float dt)
{
}

void TextureRenderTest::onRender()
{
    if(!m_renderer)
        return;

    GLCall(glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a));
    m_renderer->clear();

    //setting first logo transform and calculating MVP
    m_logo1Trans = glm::translate(glm::mat4(1.f), m_logo1Pos);
    m_view = glm::translate(glm::mat4(1.f), m_viewTranslation);
    glm::mat4 MVP1 =  m_projection * m_view * m_logo1Trans;
    auto shader = ShaderMachine::get()->getShader(ShaderType::TEXTURE_STD);
    if(!shader)
    {
        std::cout << "Shader TEXTURE_STD not found!" << std::endl;
        return;
    }
    shader->setUniform("u_MVP", MVP1);

    //making a draw call
    m_texture1->bind(0); //binding texture
    m_renderer->draw(m_vao1, ShaderType::TEXTURE_STD);

    if(m_show2NdLogo)
    {
        //setting second logo transform and calculating MVP
        m_logo2Trans = glm::translate(glm::mat4(1.f), m_logo2Pos);
        m_view = glm::translate(glm::mat4(1.f), m_viewTranslation);
        glm::mat4 MVP2 =  m_projection * m_view * m_logo2Trans;
        shader->setUniform("u_MVP", MVP2);

        //making a draw call
        m_renderer->draw(m_vao1, ShaderType::TEXTURE_STD);
    }
}

void TextureRenderTest::prepareMVP()
{
    //making starting MVP matrix
    m_view = glm::translate(glm::mat4(1.f), m_viewTranslation);
    m_logo1Trans = glm::translate(glm::mat4(1.f), m_logo1Pos);
    m_logo2Trans = glm::translate(glm::mat4(1.f), m_logo2Pos);
    m_MVP =  m_projection * m_view * m_logo1Trans;
}

void TextureRenderTest::prepareTextures()
{
    //creating vertex and index buffer objects
    m_vBuffer1 = new VertexBuffer(&m_rect1Verts, RECT_VERTBUF_SIZE * 2 * sizeof(float));
    m_iBuffer1 = new IndexBuffer(rect1Indices, 6);

    //creating vertex attributes\layout
    m_vAttributes1 = new VertexAttributes();
    m_vAttributes1->addAttribute({GL_FLOAT, 2, GL_FALSE}); //2 float coordinates
    m_vAttributes1->addAttribute({GL_FLOAT, 2, GL_FALSE}); //2 float UV coordinates

    //Packing buffers and attributes into vertex array abstraction object
    m_vao1 = new VertexAO();
    m_vao1->addBuffer(m_vBuffer1, m_iBuffer1, m_vAttributes1);
    
    //generating shader
    ShaderMachine::get()->setShader(ShaderType::TEXTURE_STD);
    m_texture1 = new Texture("resources/textures/logo.png");
    m_texture1->bind(0);
    auto shader = ShaderMachine::get()->getShader(ShaderType::TEXTURE_STD);
    if(!shader)
    {
        std::cout << "Shader TEXTURE_STD not found during preparation of textures!" << std::endl;
        return;
    }
    shader->setUniform("u_Texture", 0);
    shader->setUniform("u_MVP", m_MVP);

    //setting blend functions
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    
    //Creating Renderer
    m_renderer = Renderer::get();
}
