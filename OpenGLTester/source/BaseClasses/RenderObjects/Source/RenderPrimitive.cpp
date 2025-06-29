#include "../Public/RenderPrimitive.h"

RenderPrimitive::RenderPrimitive(const std::string& _name, std::vector<SPTR<Vertex>> _vertices,
    std::vector<size_t>&& _indices)
        :
        RenderObject(_name)
{
    for (auto&& vertex : _vertices)
        RenderObject::addVertex(std::move(vertex));
    RenderObject::addVerticesIndices(std::move(_indices));
}
