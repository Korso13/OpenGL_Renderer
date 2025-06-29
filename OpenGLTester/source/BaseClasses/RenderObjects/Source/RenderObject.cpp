#include "../Public/RenderObject.h"

RenderObject::RenderObject(const std::string& _name): Node(std::forward<const std::string&>(_name))
{}

void RenderObject::onTransformChange() {setIsDirty(true); Node::onTransformChange(); }

void RenderObject::adjustIndices(const size_t _adjustment)
{
    for(auto& index : getIndices())
    {
        index += _adjustment;
    }
}

void RenderObject::addVertex(SPTR<Vertex>&& _vertex)
{
    getVertices().push_back(std::move(_vertex));
}

void RenderObject::addVertex(Vertex&& _vertex)
{
    getVertices().emplace_back(std::make_shared<Vertex>(std::move(_vertex)));
}

void RenderObject::addVertexIndex(size_t _index)
{
    m_indices.push_back(_index);
}

void RenderObject::addVerticesIndices(std::vector<size_t>&& _indices)
{
    if (m_indices.empty())
    {
        m_indices = std::move(_indices);
    }
    else
    {
        for (auto& _index : _indices)
            m_indices.push_back(_index);
    }
}
