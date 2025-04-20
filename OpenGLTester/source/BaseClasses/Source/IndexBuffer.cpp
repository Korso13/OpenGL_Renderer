#include "../Public/IndexBuffer.h"
#include "Defines.h"
#include "Utilities/Public/GLErrorCatcher.h"
#include "BaseClasses/RenderObjects/Public/RenderObject.h"

IndexBuffer::IndexBuffer(const unsigned int* _data, const unsigned int _count, const GLenum _memUsageType)
    : m_count(_count)
{
    GLCall(glGenBuffers(1, &m_rendererId));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_count * sizeof(GLuint), _data, _memUsageType));
}

IndexBuffer::IndexBuffer(const unsigned _count)
    : m_count(_count)
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

void IndexBuffer::push_back_drawIndices(std::vector<unsigned int> _indices)
{
    m_vertexIndicesPool.insert(m_vertexIndicesPool.end(), _indices.begin(), _indices.end());
    m_count = static_cast<unsigned>(m_vertexIndicesPool.size());
}

void IndexBuffer::addRenderObject(const SPTR<RenderObject>& _RenderObject)
{
    _RenderObject->adjustIndices(m_adjustment);
    m_adjustment += _RenderObject->getVertexCount();
    auto indices = _RenderObject->getIndices();
    m_vertexIndicesPool.insert(m_vertexIndicesPool.end(), indices.begin(), indices.end());
    m_count = static_cast<unsigned>(m_vertexIndicesPool.size());
}

void IndexBuffer::bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId));
    if(!m_vertexIndicesPool.empty())
    {
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, static_cast<GLsizeiptr>(m_vertexIndicesPool.size()) * sizeof(GLuint), m_vertexIndicesPool.data());
    }

}

void IndexBuffer::unbind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
