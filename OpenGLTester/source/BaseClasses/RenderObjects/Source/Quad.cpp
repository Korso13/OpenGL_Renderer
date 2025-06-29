#include "../Public/Quad.h"
#include "../Public/RenderPrimitive.h"
#include "Utilities/Public/Math.h"

Quad::Quad(const uvec2& _size, const vec3& _position, const int _textureId, const std::string& _name)
    :
    RenderObject((_name == "Quad") ?
                 _name + std::to_string(utils::Utilities::getInternalUID()) :
                 std::forward<const std::string&>(_name)
                ),
    m_size(_size), m_textureId(_textureId)
{
    Node::setWorldPos(_position);
    makeQuad();
}

void Quad::makeQuad()
{
    if (m_isInitialized)
        return;
    
    m_isInitialized = true;
    clearVertices();
    
    //generating vertices clock-wise from bottom-left
    vec3 v1Coord = getOffsetFromCenter(0);
    SPTR<Vertex> v1 = CreateVertex(
        v1Coord,
        vec2(0.f, 0.f),
        0,
        getWorldPos() + v1Coord,
        m_textureId
    );
    addVertex(std::move(v1));
    
    vec3 v2Coord = getOffsetFromCenter(1);
    SPTR<Vertex> v2 = CreateVertex(
        v2Coord,
        vec2(0.f, 1.f),
        1,
        getWorldPos() + v2Coord,
        m_textureId
    );
    addVertex(std::move(v2));
        
    vec3 v3Coord = getOffsetFromCenter(2);
    SPTR<Vertex> v3 = CreateVertex(
        v3Coord,
        vec2(1.f, 1.f),
        2,
        getWorldPos() + v3Coord,
        m_textureId
    );
    addVertex(std::move(v3));
    
    vec3 v4Coord = getOffsetFromCenter(3);
    SPTR<Vertex> v4 = CreateVertex(
        v4Coord,
        vec2(1.f, 0.f),
        3,
        getWorldPos() + v4Coord,
        m_textureId
    );
    addVertex(std::move(v4));
    
    addVerticesIndices({
        0, 1, 2, //first triangle
        0, 2, 3  //second triangle
    });
}

void Quad::setColor(glm::vec4 _newColor)
{
    for(auto& vertex : getVertices())
    {
        if (vertex)
            vertex->color = vec4(_newColor);
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

void Quad::onTransformChange()
{
    setIsDirty(true);
    int i = 0;
    for (const auto& vertex : getVertices())
    {
        vertex->position = vec3(getWorldPos() - getOffsetFromCenter(i)*getWorldScale()); //different calculations for vertex due to anchor in the center
        i++;
    }
    Node::onTransformChange(); //skipping RenderObject::onTransformChange, because we need different logic for quad to keep anchor in the center  // NOLINT(bugprone-parent-virtual-call)
}

vec3 Quad::getOffsetFromCenter(const unsigned& _index)
{
    switch (_index)
    {
    case (0):
        return vec3(-(CAST_F(m_size.x)/2.f), -(CAST_F(m_size.y)/2.f), 0.f);
    case(1):
        return vec3(-(CAST_F(m_size.x)/2.f),  (CAST_F(m_size.y)/2.f), 0.f);
    case(2):
        return vec3( (CAST_F(m_size.x)/2.f),  (CAST_F(m_size.y)/2.f), 0.f);
    case(3):
        return vec3( (CAST_F(m_size.x)/2.f), -(CAST_F(m_size.y)/2.f), 0.f);
    default:
        return vec3();
    }
}
