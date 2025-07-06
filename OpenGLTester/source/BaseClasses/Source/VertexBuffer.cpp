#include "../Public/VertexBuffer.h"
#include "Utilities/Public/GLErrorCatcher.h"

VertexBuffer::VertexBuffer(const void* _data, const unsigned int _size, GLenum _memUsageType)
{
    GLCall(glGenBuffers(1, &m_rendererId));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererId));
    GLCall(glBufferData(GL_ARRAY_BUFFER, _size, _data, _memUsageType));
}

VertexBuffer::VertexBuffer(const unsigned int _size)
{
    GLCall(glGenBuffers(1, &m_rendererId));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererId));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * _size, nullptr, GL_DYNAMIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_rendererId));
}

void VertexBuffer::addRenderObject(const std::string& _name, const SPTR<RenderObject>& _objRef)
{
    m_renderPool.emplace(std::string(_name), _objRef);
    m_verticesToDraw += _objRef->getVertexCount();
    for(auto& vertex : _objRef->getVertices())
    {
        m_verticesPool.push_back(*vertex);
        m_verticesPool.back().textureId += CAST_F(m_verticesPool.size() - 1); //offsets renderObject textureId
    }
}

void VertexBuffer::removeRenderObject(const std::string& _name)
{
    if(!m_renderPool.contains(_name))
        return;

    m_verticesToDraw -= m_renderPool[_name].lock()->getVertexCount();
    m_renderPool.erase(_name);
    m_verticesPool.clear();
    updateVerticesPool();
}

void VertexBuffer::bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererId));
    
    if(!m_verticesPool.empty())
    {
        if (m_verticesPool.size() != m_verticesToDraw)
        {
            std::cerr << "[VertexBuffer::bind()] vertices pool and verticesToDraw size mismatch!";
            return;
        }
        GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * m_verticesToDraw, m_verticesPool.data()));
    }
}

void VertexBuffer::unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void VertexBuffer::clear()
{
    m_renderPool.clear();
    m_verticesToDraw = 0;
}

void VertexBuffer::updateVerticesPool()
{
    m_verticesPool.clear();
    std::erase_if(m_renderPool, [](const  std::pair<std::string, WPTR<RenderObject>>& _renderPoolPair)
    {
        return _renderPoolPair.second.expired();
    });
    
    for (auto& [name, objWptr] : m_renderPool)
    {
        if (objWptr.expired())
            continue;
        
        auto objRef = objWptr.lock();
        for(auto& vertex : objRef->getVertices())
        {
            m_verticesPool.push_back(*vertex); 
        }
    }
}
