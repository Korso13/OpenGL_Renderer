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

void Camera::onWindowSizeChanged(GLFWwindow* window, int width, int height)
{
    //todo: requesting self from Renderer due to being forced to use static function for resize callback. Try to remove static requirement!
    Renderer::get()->getCamera()->m_windowSize.w = width;
    Renderer::get()->getCamera()->m_windowSize.h = height;
    Renderer::get()->getCamera()->setProjectionMode( Renderer::get()->getCamera()->m_projectionMode); //updates m_projection
}

Camera::~Camera()
{
    
}

void Camera::setProjectionMode(ProjectionMode mode)
{
    m_projectionMode = mode;
    //todo: m_projectionMode init using m_windowSize
}
