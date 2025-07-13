#pragma once
#include <pch.h>
#include "Defines.h"
#include "RenderObject.h"

class RenderPrimitive;

class Quad : public RenderObject
{
public:
    Quad() = delete;
    //todo: consider adding materialInst to constructor to support multiple textures bound to various vertices (possibly not required for Quad, but other RenderObjects)
    explicit Quad(const uvec2& _size, const vec3& _position,  const std::string& _name = "Quad");
    static SPTR<Quad> build(const uvec2& _size, const vec3& _position, const std::string& _name = "Quad")
    {
        auto new_quad = M_SPTR<Quad>(_size, _position, _name);
        return new_quad;
    }
    
    Quad(const Quad& _copiedObj);
    Quad& operator=(const Quad&) = delete;
    Quad(const Quad&&) = delete;
    Quad& operator=(const Quad&&) = delete;
    
    void setColor(glm::vec4 _newColor); //todo: add shader support for functionality? Better than uniform coloring!

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
};

//Template methods impls:
template <>
inline SPTR<Quad> RenderObject::clone()
{
    return M_SPTR<Quad>(*static_cast<Quad*>(this));
}