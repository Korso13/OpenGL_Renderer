#pragma once
#include "MainMenuItem.h"
#include <PCH.h>

#define RECT_VERTBUF_SIZE 8
#define TRIANGLE_VERTBUF_SIZE 6

class VertexAO;
class VertexAttributes;
class IndexBuffer;
class Texture;
class Renderer;
class VertexBuffer;

class TextureRenderTest : public MainMenuItem
{
public:
    TextureRenderTest();
    ~TextureRenderTest() override;

    void onUpdate(float dt) override;
    void onRender() override;

private:

    void prepareMVP();
    void prepareTextures();
    
private:
    Renderer* m_renderer;

    Texture* m_texture1;
    Texture* m_texture2; //not used now
    glm::mat4 m_projection = glm::ortho(0.f, 960.f, 0.0f, 540.0f, -1.f, 1.f);
    glm::mat4 m_view;
    glm::vec3 m_viewTranslation;
    glm::mat4 m_MVP;
    glm::vec4 m_clearColor{0.f, 0.f, 0.f, 1.f};

    //vertex buffer:
    float m_rect1Verts[RECT_VERTBUF_SIZE * 2] =     //array of vertices
    {
        -50.0, -50.0,   0.0, 0.0, //one record. 2 float coordinates of vertice, 2 float UV coordinates
         50.0, -50.0,   1.0, 0.0,
         50.0,  50.0,   1.0, 1.0,
        -50.0,  50.0,   0.0, 1.0
    };
    //index buffer:
    unsigned int rect1Indices[3 * 2] =
        {
        0, 1, 2,
        0, 2, 3
    };
    VertexBuffer* m_vBuffer1;
    IndexBuffer* m_iBuffer1;
    VertexAttributes* m_vAttributes1;
    VertexAO* m_vao1;
    
    bool m_show2NdLogo = false;
    glm::vec3 m_logo1Pos = glm::vec3(60.f, 60.f, 0.f);
    glm::mat4 m_logo1Trans = glm::mat4(1.f);
    glm::vec3 m_logo2Pos = glm::vec3(60.f, 60.f, 0.f);
    glm::mat4 m_logo2Trans = glm::mat4(1.f);
};
