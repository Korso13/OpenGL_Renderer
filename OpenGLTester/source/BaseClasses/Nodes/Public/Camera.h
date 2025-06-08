#pragma once

#include "Node.h"

class Renderer;

class Camera : public Node
{
    friend Renderer;

public:

    enum class ProjectionMode
    {
        ORTHOGRAFIC, //no z depth
        PERSPECTIVE  //with z depth (objects in distance look smaller)
    };
    
public:

    ~Camera() override;

    void setProjectionMode(ProjectionMode mode);
    
private:

    Camera(); //should only be called by Renderer

    static void onWindowSizeChanged(GLFWwindow* window, int width, int height); //todo: find a way to avoid static
    
private:

    ProjectionMode m_projectionMode = ProjectionMode::ORTHOGRAFIC;
    ivec2 m_windowSize;
    //boundaries of virtual space(viewport) in ortho projection
    glm::mat4 m_projection = glm::ortho(0.f, 1280.f, 0.0f, 800.0f, -1.f, 1.f);
    //Camera projection(vertices shift from camera frustum)
    glm::mat4 m_view;
    //Camera position
    glm::vec3 m_viewTranslation;   

    //clear color
    glm::vec4 m_clearColor{0.f, 0.f, 0.f, 1.f};
};
