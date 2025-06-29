#pragma once
#include <pch.h>
#include "Defines.h"
#include "RenderObject.h"

class RenderPrimitive;

class Quad : public RenderObject
{
public:
    Quad() = delete;
    Quad(const uvec2& _size, const vec3& _position, const int _textureId = -1, const std::string& _name = "Quad");
    static SPTR<Quad> build(const uvec2& _size, const vec3& _position, const int _textureId = -1, const std::string& _name = "Quad")
    {
        auto new_quad = SPTR<Quad>(new Quad(_size, _position, _textureId, _name));
        return new_quad;
    }
    //todo: add copy/move constructors?
    
    void setColor(glm::vec4 _newColor); //todo: remove when MaterialInst/Texture can replace functionality?

protected:

    void addVertex(SPTR<Vertex>&& _vertex) override;
    void addVertex(Vertex&& _vertex) override;

    void onTransformChange() override;
    
private:

    void makeQuad();
    vec3 getOffsetFromCenter(const unsigned int& _index);
    
private:

    bool m_isInitialized = false;
    const uvec2 m_size;
    int32_t m_textureId; //todo: is it needed?
};
