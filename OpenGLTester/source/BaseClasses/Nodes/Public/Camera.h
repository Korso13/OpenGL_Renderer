#pragma once

#include "Node.h"

class Renderer;

class Camera : public Node
{
    friend class Renderer;
    friend class RendererBatch;

public:

    enum class ProjectionMode
    {
        ORTHOGRAFIC, //no z depth
        PERSPECTIVE  //with z depth (objects in distance look smaller)
    };
    
public:

    ~Camera() override;

    void setProjectionMode(const ProjectionMode _mode);

    [[nodiscard]] float getFov() const {return m_fov;}
    void setFov(const float _newFov){m_fov = _newFov;}
    [[nodiscard]] float getPerspectiveNear() const { return m_perspectiveNear; }
    void setPerspectiveNear(const float _perspectiveNear) { m_perspectiveNear = _perspectiveNear; }
    [[nodiscard]] float getPerspectiveFar() const { return m_perspectiveFar; }
    void setPerspectiveFar(const float _perspectiveFar) { m_perspectiveFar = _perspectiveFar; }
    void setClearColor(const vec3& _newClColor);
    void setClearColor(const vec4& _newClColor);
    void setClearColor(glm::vec4 _newClColor);
    
    void setEnabled(const bool _isEnabled) override {}
    [[nodiscard]] bool isEnabled() const override {return true;}
    bool isInFrustum() const override {return true;}
    
    void setWorldScale(const vec3 _newScale) override {} //NOTE: scale is always 1. Probably correct, but might affect HUD elements' scaling
    void setLocalScale(const vec3 _newScale) override {}

protected:
    
    void onTransformChange() override;

private:

    Camera(); //should only be called by Renderer or RendererBatch

    //Used by Renderer:
    [[nodiscard]] glm::mat4 getMvp() const; //uses zero-position model (moving vertices, instead of model)
    [[nodiscard]] glm::mat4 getMvpForRender(const Node& _nodeToRender) const; //uses model position from node 
    [[nodiscard]] glm::mat4 getMvpForRender(const SPTR<Node>& _nodeToRender) const; //uses model position from node
    
    void updateProjection();

    static void onWindowSizeChanged(GLFWwindow* _window, const int _width, const int _height); //todo: find a way to avoid static
    
private:

    ProjectionMode m_projectionMode = ProjectionMode::ORTHOGRAFIC;
    
    ivec2 m_windowSize;
    float m_fov = 70.f;
    float m_perspectiveNear = 0.1f;
    float m_perspectiveFar = 100.f;
    
    //boundaries of virtual space(viewport) in ortho projection
    glm::mat4 m_projection = glm::ortho(0.f, 1280.f, 0.0f, 800.0f, -1.f, 1.f);
    //Camera projection(vertices shift from camera frustum)
    glm::mat4 m_view;

    //clear color
    glm::vec4 m_clearColor{0.f, 0.f, 0.f, 1.f};
};
