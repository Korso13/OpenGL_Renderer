#include "../Public/Quad.h"
#include "../Public/RenderPrimitive.h"
#include "Utilities/Public/Math.h"

Quad::Quad(glm::uvec2 _size, glm::vec3 _position, int _textureID, const std::string& _name)
    :
    RenderObject((_name == "Quad") ?
                 _name + std::to_string(utils::Utilities::getInternalUID()) :
                 std::forward<const std::string&>(_name)
                )
{
    makeQuad(_size, _position, _textureID);
}

void Quad::makeQuad(glm::uvec2 _size, glm::vec3 _position, int _textureID)
{
    m_size = _size;
    m_position = vec3(_position);
    m_textureID = _textureID;
    clearVertices();
    
    //generating vertices clock-wise
    SPTR<Vertex> v1 = std::make_shared<Vertex>(*(new Vertex));
    v1->Position = m_position + vec3(-(CAST_F(_size.x)/2.f), -(CAST_F(_size.y)/2.f), 0.f);
    v1->UV = vec2(0.f, 0.f);
    v1->TextureID = static_cast<float>(_textureID);
    v1->VertexIndex = 0;
    addVertex(std::move(v1));
    
    SPTR<Vertex> v2 = std::make_shared<Vertex>(*(new Vertex));
    v2->Position = m_position + vec3(-(CAST_F(_size.x)/2.f), (CAST_F(_size.y)/2.f), 0.f);
    v2->UV = vec2(0.f, 1.f);
    v2->TextureID = static_cast<float>(_textureID);
    v2->VertexIndex = 1;
    addVertex(std::move(v2));
        
    SPTR<Vertex> v3 = std::make_shared<Vertex>(*(new Vertex));
    v3->Position = m_position + vec3((CAST_F(_size.x)/2.f), (CAST_F(_size.y)/2.f), 0.f);
    v3->UV = vec2(1.f, 1.f);
    v3->TextureID = static_cast<float>(_textureID);
    v3->VertexIndex = 2;
    addVertex(std::move(v3));
    
    SPTR<Vertex> v4 = std::make_shared<Vertex>(*(new Vertex));
    v4->Position = m_position + vec3((CAST_F(_size.x)/2.f), -(CAST_F(_size.y)/2.f), 0.f);
    v4->UV = vec2(1.f, 0.f);
    v4->TextureID = static_cast<float>(_textureID);
    v4->VertexIndex = 3;
    addVertex(std::move(v4));
    
    addVerticesIndices({
        0, 1, 2, //first triangle
        0, 2, 3  //second triangle
    });
}

/*void Quad::setPosition(glm::vec3& _newPosition)
{
    //return;
    int i = 0;
    for (auto& vertex : m_vertices)
    {
        vertex->Position =
            vec3(CAST_F(m_size.x) * vertex->UV.x, CAST_F(m_size.y) * vertex->UV.y, 0.f) +
            getOffsetFromCenter(i) +
            vec3(_newPosition);
        i++;
    }
}*/

void Quad::setColor(glm::vec4 newColor)
{
    for(auto& vertex : getVertices())
    {
        if (vertex)
            vertex->Color = vec4(newColor);
    }
}

void Quad::addVertex(SPTR<Vertex>&& _vertex)
{
    if(getVertices().size() >= 4)
        return;
    
   addVertex(std::move(_vertex));
}

void Quad::addVertex(Vertex&& _vertex)
{
    if(getVertices().size() >= 4)
        return;
    
    addVertex(std::move(_vertex));
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
