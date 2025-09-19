#include "../Public/Camera.h"

#include "ManagingClasses/Public/Renderer.h"
#include "Utilities/Public/ImG_AutoDrawers.h"

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
    setWorldPos(vec3(0.f));
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
    std::cout << "[Camera::updateProjection] Updating " << ((m_projectionMode == ProjectionMode::ORTHOGRAFIC) ? "Ortho " : "Perspective ") << 
        "projection with window size " <<
        m_windowSize.w << "x" << m_windowSize.h << "\n";
    
    if(m_projectionMode == ProjectionMode::ORTHOGRAFIC)
    {
        m_projection = glm::ortho(0.f, CAST_F(m_windowSize.w), 0.0f, CAST_F(m_windowSize.h), -1.f, 1.f);
    }
    else
    {
        m_projection = glm::perspective(m_fov, (CAST_F(m_windowSize.w)/CAST_F(m_windowSize.h)), m_perspectiveNear, m_perspectiveFar);
    }
}

bool Camera::onGui(const std::string& _name)
{
    bool result = false;
    result = result || AutoDrawers::DrawClassVariables("Camera",
        NamedVar<float*>{"FOV", &m_fov},
        NamedVar<glm::vec4*>{"Clear color", &m_clearColor},
        NamedVar<ivec2*>{"Window size", &m_windowSize},
        NamedVar<float*>{"Perspective Near", &m_perspectiveNear},
        NamedVar<float*>{"Perspective Far", &m_perspectiveFar}
    );
    if (result)
    {
        updateProjection();
        GLCall(glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a));
    }
    result = result || Node::onGui(_name);
    return result;
}

void Camera::onTransformChange()
{
    Node::onTransformChange();
    
    const vec3 world_pos = -getWorldPos();
    m_view = glm::translate(glm::mat4(1.f), world_pos.glmFormat());
}

void Camera::setProjectionMode(const ProjectionMode _mode)
{
    m_projectionMode = _mode;
    updateProjection();
}

void Camera::setClearColor(const vec3& _newClColor)
{
    m_clearColor = glm::vec4(_newClColor.x, _newClColor.y, _newClColor.z, 1.f);
    GLCall(glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a));
}

void Camera::setClearColor(const vec4& _newClColor)
{
    m_clearColor = glm::vec4(_newClColor.x, _newClColor.y, _newClColor.z, _newClColor.a);
    GLCall(glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a));
}

void Camera::setClearColor(const glm::vec4 _newClColor)
{
    m_clearColor = _newClColor;
    GLCall(glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a));
}

glm::mat4 Camera::getMvp() const
{
    const glm::mat4 model = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, 0.f)); //instead of shifting the entire model plane, we shift vertices' coordinates
    const glm::mat4 mvp =  m_projection * m_view * model;

    return mvp;
}

glm::mat4 Camera::getMvpForRender(const Node& _nodeToRender) const
{
    const vec3 world_pos = _nodeToRender.getWorldPos();
    const glm::mat4 model = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, 0.f)); //instead of shifting the entire model plane, we shift vertices' coordinates
    const glm::mat4 mvp =  m_projection * m_view * model;

    return mvp;
}

glm::mat4 Camera::getMvpForRender(const SPTR<Node>& _nodeToRender) const
{
    if(_nodeToRender)
        return getMvpForRender(*_nodeToRender);
    else
    {
        std::cerr << "[Camera::getMvpForRender] Invalid node! Returning default mvp matrix!" << std::endl;
        return glm::mat4();
    }
}
