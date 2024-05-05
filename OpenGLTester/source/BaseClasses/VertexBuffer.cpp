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
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererId));
    
    if(!m_renderPool.empty())
    {
        Vertex* vertices = new Vertex[m_VerticesToDraw];
        float* verticesAlt = new float[m_VerticesToDraw*12];
        size_t VertexCounter = 0;
        size_t VertexCounterAlt = 0;
        for(auto& object : m_renderPool)
        {
            for(auto& vertex : object.second->getVertices())
            {
                if(VertexCounter > (m_VerticesToDraw - 1))
                {
                    std::cout << "Vertex buffer binding error! Tried to bind more vertices than registered!" << std::endl;
                    continue;
                }
                
                //filling vertices array with data from Vertex structs provided by renderable objects
                vertices[VertexCounter] = *vertex.get();
                std::cout << "Debug vertex " << VertexCounter << " data. Vertex index: " << vertices[VertexCounter].VertexIndex << std::endl;
                //std::cout << "Debug vertex origin data. Vertex index: " << vertex.get()->VertexIndex << std::endl;
                verticesAlt[VertexCounterAlt] = vertex->Position.x;
                verticesAlt[VertexCounterAlt+1] = vertex->Position.y;
                verticesAlt[VertexCounterAlt+2] = vertex->Position.z;
                verticesAlt[VertexCounterAlt+3] = vertex->Position.a;
                verticesAlt[VertexCounterAlt+4] = vertex->Color.x;
                verticesAlt[VertexCounterAlt+5] = vertex->Color.y;
                verticesAlt[VertexCounterAlt+6] = vertex->Color.z;
                verticesAlt[VertexCounterAlt+7] = vertex->Color.a;
                verticesAlt[VertexCounterAlt+8] = vertex->UV.x;
                verticesAlt[VertexCounterAlt+9] = vertex->UV.y;
                verticesAlt[VertexCounterAlt+10] = vertex->TextureID;
                verticesAlt[VertexCounterAlt+11] = static_cast<float>(vertex->VertexIndex);
                VertexCounterAlt +=12;
                VertexCounter++;
            }
        }

        GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * m_VerticesToDraw, vertices));
    }
    
}

void VertexBuffer::unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
