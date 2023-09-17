#include "VertexBuffer.h"
#include "Utilities/GLErrorCatcher.h"

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

void VertexBuffer::addRenderObject(const std::string& _name,  SPTR<RenderObject> _objRef)
{
    m_renderPool.emplace(std::string(_name), _objRef);
    m_VerticesToDraw += _objRef->getVertexCount();
}

void VertexBuffer::removeRenderObject(const std::string& _name)
{
    if(m_renderPool.find(_name) == m_renderPool.end())
        return;

    m_VerticesToDraw -= m_renderPool[_name]->getVertexCount();
    m_renderPool.erase(_name);
}

void VertexBuffer::bind() const
{
    if(!m_renderPool.empty())
    {
        Vertex* vertices = new Vertex[m_VerticesToDraw];
        size_t VertexCounter = 0;
        for(auto& object : m_renderPool)
        {
            for(auto& vertex : object.second->getVertexes())
            {
                if(VertexCounter > (m_VerticesToDraw - 1))
                {
                    std::cout << "Vertex buffer binding error! Tried to bind more vertices than registered!" << std::endl;
                    continue;
                }
                
                //filling vertices array with data from Vertex structs provided by renderable objects
                vertices[VertexCounter] = *vertex.get();
                VertexCounter++;
            }
        }

        GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * m_VerticesToDraw, vertices));
    }
    
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererId));
}

void VertexBuffer::unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
