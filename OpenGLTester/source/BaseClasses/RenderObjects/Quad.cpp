#include "Quad.h"
#include "RenderPrimitive.h"


Quad::Quad(glm::uvec2 _size, glm::vec3 _position, int _textureID)
{
    makeQuad(_size, _position, _textureID);
}

void Quad::makeQuad(glm::uvec2 _size, glm::vec3 _position, int _textureID)
{
    m_size = _size;
    m_position = vec3(_position);
    m_textureID = _textureID;
    m_vertices.clear();
    
    //generating vertices clock-wise
    SPTR<Vertex> v1 = std::make_shared<Vertex>(*(new Vertex));
    v1->Position = m_position + vec3(-(_size.x/2.f), -(_size.y/2.f), 0.f);
    v1->UV = vec2(0.f, 0.f);
    v1->TextureID = static_cast<float>(_textureID);
    v1->VertexIndex = 0;
    m_vertices.push_back(v1);
    
    SPTR<Vertex> v2 = std::make_shared<Vertex>(*(new Vertex));
    v2->Position = m_position + vec3(-(_size.x/2.f), (_size.y/2.f), 0.f);
    v2->UV = vec2(0.f, 1.f);
    v2->TextureID = static_cast<float>(_textureID);
    v2->VertexIndex = 1;
    m_vertices.push_back(v2);
        
    SPTR<Vertex> v3 = std::make_shared<Vertex>(*(new Vertex));
    v3->Position = m_position + vec3((_size.x/2.f), (_size.y/2.f), 0.f);
    v3->UV = vec2(1.f, 1.f);
    v3->TextureID = static_cast<float>(_textureID);
    v3->VertexIndex = 2;
    m_vertices.push_back(v3);
    
    SPTR<Vertex> v4 = std::make_shared<Vertex>(*(new Vertex));
    v4->Position = m_position + vec3((_size.x/2.f), -(_size.y/2.f), 0.f);
    v4->UV = vec2(1.f, 0.f);
    v4->TextureID = static_cast<float>(_textureID);
    v4->VertexIndex = 3;
    m_vertices.push_back(v4);
    
    m_indices = {
        0, 1, 2,
        0, 2, 3
    };
}

void Quad::setPosition(glm::vec3& _newPosition)
{
    //return;
    int i = 0;
    for (auto& vertex : m_vertices)
    {
        vertex->Position =
            vec3(m_size.x * vertex->UV.x, m_size.y * vertex->UV.y, 0.f) +
            getOffsetFromCenter(i) +
            vec3(_newPosition);
        i++;
    }
}

void Quad::setColor(glm::vec4 newColor)
{
    for(auto& vertex : m_vertices)
    {
        vertex->Color = vec4(newColor);
    }
}

void Quad::addPolygon(SPTR<RenderPrimitive> _polygon)
{
    for(auto& vertex : _polygon->getVertices())
    {
        addVertex(std::move(vertex));
    }
}

void Quad::addVertex(SPTR<Vertex> _vertex)
{
    if(m_vertices.size() >= 4)
        return;
    
    m_vertices.push_back(_vertex);
}

void Quad::addVertex(Vertex&& _vertex)
{
    if(m_vertices.size() >= 4)
        return;
    
    m_vertices.push_back(std::make_shared<Vertex>(_vertex));
}

void Quad::adjustIndices(unsigned long long _adjustment)
{
    for(auto& index : m_indices)
    {
        index += _adjustment;
    }
}

vec3 Quad::getOffsetFromCenter(const unsigned& index)
{
    switch (index)
    {
    case (0):
        return vec3(-(m_size.x/2.f), -(m_size.y/2.f), 0.f);
    case(1):
        return vec3(-(m_size.x/2.f),  (m_size.y/2.f), 0.f);
    case(2):
        return vec3( (m_size.x/2.f),  (m_size.y/2.f), 0.f);
    case(3):
        return vec3( (m_size.x/2.f), -(m_size.y/2.f), 0.f);
    default:
        return vec3();
    }
}
