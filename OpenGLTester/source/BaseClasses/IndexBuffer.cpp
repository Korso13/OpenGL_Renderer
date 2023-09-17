#include "IndexBuffer.h"
#include "Defines.h"
#include "Utilities/GLErrorCatcher.h"
#include "BaseClasses/RenderObjects/RenderObject.h"

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
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_count * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_rendererId));
}

void IndexBuffer::push_back_drawIndices(std::vector<unsigned int> _indices)
{
    m_vertexIndicesPool.insert(m_vertexIndicesPool.end(), _indices.begin(), _indices.end());
}

void IndexBuffer::push_back_drawIndices(const SPTR<RenderObject>& _RenderObject)
{
    _RenderObject->adjustIndices(m_vertexIndicesPool.size());
    push_back_drawIndices(_RenderObject->getIndices());
}

void IndexBuffer::bind() const
{
    if(!m_vertexIndicesPool.empty())
    {
        unsigned int* Indices = new unsigned int[m_vertexIndicesPool.size()];
        size_t i = 0;
        for (auto& index : m_vertexIndicesPool)
        {
            Indices[i] = index;
            i++;
        }

        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_vertexIndicesPool.size() * sizeof(Vertex), Indices);
    }

    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId));
}

void IndexBuffer::unbind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
