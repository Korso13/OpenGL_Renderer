#pragma once
#include <pch.h>
#include "Defines.h"
#include "RenderObject.h"

class RenderPrimitive;

class Quad : public RenderObject
{
public:
    Quad() = delete;
    explicit Quad(const std::string& _name = "Quad");
    Quad(glm::uvec2 _size, glm::vec3 _position, int _textureID = -1, const std::string& _name = "Quad");
    //todo: does it require destructor?
    
    void makeQuad(glm::uvec2 _size, glm::vec3 _position, int _textureID = -1);

    //todo: remove/rewrite using Node parent
    vec3 getPosition() const {return m_position;}
    void setPosition(glm::vec3& _newPosition);
    
    void setColor(glm::vec4 newColor); //todo: remove when MaterialInst/Texture can replace functionality?
    
    void addPolygon(SPTR<RenderPrimitive> _polygon);
    
    void adjustIndices(size_t _adjustment) override;

protected:
    void addVertex(SPTR<Vertex> _vertex) override;
    void addVertex(Vertex&& _vertex) override;
    
private:

    vec3 getOffsetFromCenter(const unsigned int& index);
    
private:
    
    glm::uvec2 m_size{};
    vec3 m_position; //todo: replace with call to Node
    unsigned int m_textureID{};
};
