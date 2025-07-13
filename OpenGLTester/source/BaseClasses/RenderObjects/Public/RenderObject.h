#pragma once
#include "Defines.h"
#include <pch.h>
#include "BaseClasses/Materials/Public/MaterialInstance.h"
#include "BaseClasses/Nodes/Public/Node.h"

class VertexBuffer;
class IndexBuffer;
class RendererBatch;

class RenderObject : public Node
{
    friend RendererBatch;
    friend IndexBuffer;
    friend VertexBuffer;
    
public:
    RenderObject() = delete;
    explicit RenderObject(const std::string& _name);
    RenderObject(const RenderObject& _copiedObj); //copy ctor
    RenderObject& operator=(const RenderObject& _copiedObj) = delete; //copy assignment ctor deleted for now 
    RenderObject(const RenderObject&& _copiedObj) = delete;
    RenderObject& operator=(const RenderObject&& _copiedObj) = delete;
    ~RenderObject() override = default;

    //Returns object's render order priority
    virtual uint32_t getRenderOrder() {return m_renderOrder;}
    //Sets object's render order priority
    virtual void setRenderOrder(const uint32_t _newRO);

    //returns material instance (ideally not overriden)
    virtual SPTR<MaterialInstance> getMatInst() {return m_materialInstance;}
    //sets new material instance (ideally not overriden)
    virtual void setMatInst(const SPTR<MaterialInstance> _newMI);
    
    bool isInBatch() const {return m_isInBatch;}

    //SPTR-based cloning. Uses copy ctor!
    //todo: consider moving up to Object class
    template<typename ClonedType>
    requires std::is_copy_constructible_v<ClonedType>
    [[nodiscard]] SPTR<ClonedType> clone()
    {
        //static_assert(false, "You must add specialization of clone() for your class and implement copy ctor for it!");
        return M_SPTR<ClonedType>(*static_cast<ClonedType*>(this));
    }

protected:

    //Render-related methods (implemented and used by heir classes)
    virtual void addVertex(SPTR<Vertex>&& _vertex);
    virtual void addVertex(Vertex&& _vertex);
    std::vector<SPTR<Vertex>>& getVertices() {return m_vertices;}
    virtual size_t getVertexCount() const {return m_vertices.size();}
    void clearVertices(){m_vertices.clear();}

    virtual void addVertexIndex(size_t _index);
    virtual void addVerticesIndices(std::vector<size_t>&& _indices);
    const std::vector<size_t>& getIndices() {return m_indices;}
    
    bool isDirty() const { return m_isDirty; }
    void setIsDirty(const bool _isDirty) { m_isDirty = _isDirty; }

    void onTransformChange() override;

    
private:
    
    bool m_isDirty = true; // set to "false" after being put in batch, reset to "true" after render parameters change
    bool m_isInBatch = false; //if false - needs to be placed in a batch
    uint32_t m_renderOrder = 0;
    SPTR<MaterialInstance> m_materialInstance = nullptr;

    std::vector<SPTR<Vertex>> m_vertices;
    std::vector<size_t> m_indices;
};

//Template methods impls:
template <>
inline SPTR<RenderObject> RenderObject::clone<RenderObject>()
{
    return M_SPTR<RenderObject>(*this);
}