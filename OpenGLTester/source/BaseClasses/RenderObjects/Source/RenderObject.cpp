#include "../Public/RenderObject.h"
#include "Utilities/Public/Math.h"

RenderObject::RenderObject(const std::string& _name): Node(std::forward<const std::string&>(_name))
{}

void RenderObject::setRenderOrder(const uint32_t _newRO)
{
    m_renderOrder = _newRO;
    setIsDirty(true);
}

void RenderObject::setMatInst(const SPTR<MaterialInstance> _newMI)
{
    m_materialInstance = _newMI;
    setIsDirty(true);
}

void RenderObject::onTransformChange()
{
    setIsDirty(true);
    for(const auto& vertex : m_vertices)
    {
        vertex->position = getWorldPos() + vertex->vCoord * getWorldScale();
    }
    Node::onTransformChange();
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
