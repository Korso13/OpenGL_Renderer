#include "../Public/RenderObject.h"

#include "Utilities/Public/ImG_AutoDrawers.h"
#include "Utilities/Public/Math.h"

RenderObject::RenderObject(const std::string& _name): Node(std::forward<const std::string&>(_name))
{}

RenderObject::RenderObject(const RenderObject& _copiedObj)
    :
    Node(std::forward<const std::string>(_copiedObj.getName())), //instancing new node, Node-cloning is disabled for now
    m_isDirty(true),
    m_isInBatch(false),
    m_renderOrder(_copiedObj.m_renderOrder),
    m_materialInstance(_copiedObj.m_materialInstance), //common MaterialInstance is ok in some cases
    m_indices(_copiedObj.m_indices)
{
    //since vertices are kept as SPTR, need to clone them this way
    for (auto& vertex : _copiedObj.m_vertices)
    {
        m_vertices.emplace_back(M_SPTR<Vertex>(*vertex));
    }
}

RenderObject::~RenderObject()
{

}

void RenderObject::setRenderOrder(const uint32_t _newRO)
{
    m_renderOrder = _newRO;
    setIsDirty(true);
}

void RenderObject::setMatInst(const SPTR<MaterialInstance> _newMI, TextureId _objectTextureId)
{
    m_materialInstance = _newMI;
    if (_objectTextureId != TextureId::SIZE)
    {
        m_objectTextureId = _objectTextureId;
        setAllVerticesTextureId(_objectTextureId);
    }
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

bool RenderObject::onGui(const std::string& _name)
{
    bool result = false;
    result = result || AutoDrawers::DrawClassVariables("RenderObject",
        NamedVar<bool*>{"is Dirty", &m_isDirty},
        NamedVar<bool*>{"Is in Batch", &m_isInBatch},
        NamedVar<uint32_t*>{"Render Order", &m_renderOrder},
        NamedVar<MaterialInstance*>{"Material Instance", m_materialInstance.get()},
        NamedVar<TextureId*>{"Object Texture", &m_objectTextureId},
        NamedContainer<std::vector<SPTR<Vertex>>*>{"Verticies", &m_vertices},
        NamedContainer<std::vector<size_t>*>{"Indices", &m_indices},
        SubMenu{_name, SUB_MENU_CALL(return result = result || Node::onGui(_name);)}
    );
    return result;
}

void RenderObject::addVertex(SPTR<Vertex>&& _vertex)
{
    getVertices().push_back(std::move(_vertex));
}

void RenderObject::addVertex(Vertex&& _vertex)
{
    getVertices().emplace_back(std::make_shared<Vertex>(std::move(_vertex)));
}

void RenderObject::setAllVerticesTextureId(TextureId _objectsTextureId)
{
    for (const auto& vertex : m_vertices)
        vertex->textureId = CAST_F(_objectsTextureId);
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
