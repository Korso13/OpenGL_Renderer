#include "../Public/IndexBuffer.h"

#include <algorithm>

#include "Defines.h"
#include "Utilities/Public/GLErrorCatcher.h"
#include "BaseClasses/RenderObjects/Public/RenderObject.h"
#include "Utilities/Public/ImG_AutoDrawers.h"

IndexBuffer::IndexBuffer(const unsigned int* _data, const size_t _count, const GLenum _memUsageType)
    :
EngineInternal("IndexBuffer"), m_count(_count)
{
    GLCall(glGenBuffers(1, &m_rendererId));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_count * sizeof(GLuint), _data, _memUsageType));
}

IndexBuffer::IndexBuffer(const size_t _count)
    :
EngineInternal("IndexBuffer"), m_count(_count)
{
    GLCall(glGenBuffers(1, &m_rendererId));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_count * sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW));
    m_count = 0;
}

IndexBuffer::~IndexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_rendererId));
}

void IndexBuffer::addRenderObject(const SPTR<RenderObject>& _renderObject)
{
    for(auto& index : _renderObject->getIndices())
    {
        m_vertexIndicesPool.push_back(GLuint(index) + GLuint(m_adjustment));
    }
    //updating adjustment
    m_adjustment += _renderObject->getVertexCount();
    
    m_count = m_vertexIndicesPool.size();
}

void IndexBuffer::bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId));
    if(!m_vertexIndicesPool.empty())
    {
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_vertexIndicesPool.size() * sizeof(GLuint), m_vertexIndicesPool.data());
    }

}

void IndexBuffer::unbind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void IndexBuffer::clear()
{
    m_vertexIndicesPool.clear(); m_adjustment = 0; m_count = 0;
}

bool IndexBuffer::onGui(const std::string& _name)
{
    bool result = false;
    result = result || AutoDrawers::DrawClassVariables("IndexBuffer",
        NamedVar<size_t*>{"Indices count", &m_count},
        NamedVar<size_t*>{"Index adjustment", &m_adjustment},
        NamedContainer<std::vector<GLuint>*>{"Indices pool", &m_vertexIndicesPool},
        SubMenu{_name, SUB_MENU_CALL(return result = result || EngineInternal::onGui(_name);)}
    );
    return result;
}
