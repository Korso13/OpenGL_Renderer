#pragma once
#include <pch.h>
#include "Defines.h"
#include "RenderObject.h"

class RenderPrimitive;

class Quad : public RenderObject
{
public:

    Quad() = default;
    Quad(glm::uvec2 _size, glm::vec3 _position, int _textureID = -1);

    void makeQuad(glm::uvec2 _size, glm::vec3 _position, int _textureID = -1);
    vec3 getPosition() const {return m_position;}
    void setPosition(glm::vec3& _newPosition);
    void setColor(glm::vec4 newColor);
    
    void addPolygon(SPTR<RenderPrimitive> _polygon);
    void addVertex(SPTR<Vertex> _vertex) override;
    void addVertex(Vertex&& _vertex) override;
    
    std::vector<SPTR<Vertex>> getVertices() override {return m_vertices;}
    size_t getVertexCount() const override {return m_vertices.size();}
    std::vector<unsigned int> getIndices() override {return m_indices;}
    void adjustIndices(unsigned int _adjustment) override;

private:

    vec3 getOffsetFromCenter(const unsigned int& index);
    
private:
    std::vector<SPTR<Vertex>> m_vertices;
    std::vector<unsigned int> m_indices;
    
    glm::uvec2 m_size;
    vec3 m_position;
    unsigned int m_textureID;
};
