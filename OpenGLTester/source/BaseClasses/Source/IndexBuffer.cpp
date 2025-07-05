#include "../Public/IndexBuffer.h"

#include <algorithm>

#include "Defines.h"
#include "Utilities/Public/GLErrorCatcher.h"
#include "BaseClasses/RenderObjects/Public/RenderObject.h"

IndexBuffer::IndexBuffer(const unsigned int* _data, const size_t _count, const GLenum _memUsageType)
    : m_count(_count)
{
    GLCall(glGenBuffers(1, &m_rendererId));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_count * sizeof(GLuint), _data, _memUsageType));
}

IndexBuffer::IndexBuffer(const size_t _count)
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

void IndexBuffer::addRenderObject(const SPTR<RenderObject>& _renderObject)
{
    std::vector<size_t> new_indices = _renderObject->getIndices(); //copying intended!
    //adjusting indices values
    std::transform(new_indices.begin(), new_indices.end(), new_indices.begin(),
        [this](const size_t _index)
            {
                return _index + m_adjustment;
            });
    //updating adjustment
    m_adjustment += _renderObject->getVertexCount();
    m_vertexIndicesPool.insert(m_vertexIndicesPool.end(), new_indices.begin(), new_indices.end());
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
