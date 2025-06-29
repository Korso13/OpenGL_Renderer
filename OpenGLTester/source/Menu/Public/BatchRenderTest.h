#pragma once
#include <pch.h>
#include "Defines.h"
#include "MainMenuItem.h"

class Quad;
class VertexAO;
class VertexAttributes;
class IndexBuffer;
class Texture;
class Renderer;
class VertexBuffer;

class BatchRenderTest : public MainMenuItem
{
public:
    BatchRenderTest();
    
    void onUpdate(float dt) override;
    void onRender() override;

private:

    void prepareMVP();
    void prepareTextures();
    void updateQuadsTranslations();
    
private:

    Renderer* m_renderer;
    
    Texture* m_texture1;
    Texture* m_texture2;
    glm::mat4 m_projection = glm::ortho(0.f, 1280.f, 0.0f, 800.0f, -1.f, 1.f); //boundaries of virtual space(viewport) in ortho projection
    glm::mat4 m_view;               //Camera projection(vertices shift from camera frustum)
    glm::vec3 m_viewTranslation;    //Camera position
    glm::mat4 m_MVP;                //Model View Projection
    glm::vec4 m_clearColor{0.f, 0.f, 0.f, 1.f};

    SPTR<Quad> m_logoPlane1;
    SPTR<Quad> m_logoPlane2;
    SPTR<VertexBuffer> m_vBuffer;
    SPTR<IndexBuffer> m_iBuffer;
    SPTR<VertexAttributes> m_vAttributes;
    SPTR<VertexAO> m_vao;
    
    vec3 m_logo1Pos = vec3(60.f, 60.f, 0.f);
    vec3 m_logo2Pos = vec3(600.f, 60.f, 0.f);

    int m_debugTexIndex = -1;
};
