#include "../Public/Camera.h"

#include "ManagingClasses/Public/Renderer.h"

Camera::Camera()
    :
    Node("ScreenCamera")
{
    glfwGetWindowSize(glfwGetCurrentContext(), &m_windowSize.w, &m_windowSize.h);
    //void (*resizeFunHolder)(GLFWwindow* window, int width, int height) = std::bind(&Camera::onWindowSizeChanged, this, std::placeholders::_3);
    //std::function<void(GLFWwindow* window, int width, int height)> resizeFunHolder = std::bind(&Camera::onWindowSizeChanged, this, std::placeholders::_3);
    //auto resizeFunHolder = this->Camera::onWindowSizeChanged;
    glfwSetWindowSizeCallback(glfwGetCurrentContext(), &Camera::onWindowSizeChanged);
    setProjectionMode(m_projectionMode);
}

Camera::~Camera()
{
    
}

void Camera::onWindowSizeChanged(GLFWwindow* _window, const int _width, const int _height)
{
    //todo: requesting self from Renderer due to being forced to use static function for resize callback. Try to remove static requirement!
    Renderer::get()->getCamera()->m_windowSize.w = _width;
    Renderer::get()->getCamera()->m_windowSize.h = _height;
    Renderer::get()->getCamera()->updateProjection(); //updates m_projection
}

void Camera::updateProjection()
{
    if(m_projectionMode == ProjectionMode::ORTHOGRAFIC)
    {
        m_projection = glm::ortho(0.f, CAST_F(m_windowSize.w), 0.0f, CAST_F(m_windowSize.h), -1.f, 1.f);
    }
    else
    {
        m_projection = glm::perspective(m_fov, (CAST_F(m_windowSize.w)/CAST_F(m_windowSize.h)), m_perspectiveNear, m_perspectiveFar);
    }
}

void Camera::onTransformChange()
{
    Node::onTransformChange();
    //todo: update m_viewTranslation
}

void Camera::setProjectionMode(const ProjectionMode _mode)
{
    m_projectionMode = _mode;
    updateProjection();
}
