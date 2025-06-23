#pragma once
#include "Defines.h"
#include <pch.h>
#include "BaseClasses/Materials/Public/MaterialInstance.h"
#include "BaseClasses/Nodes/Public/Node.h"

class RendererBatch;

class RenderObject : public Node
{
    friend RendererBatch;
    
public:
    RenderObject() = delete;
    explicit RenderObject(const std::string& _name) : Node(std::forward<const std::string&>(_name)) {}
    ~RenderObject() override = default;

    //Returns object's render order priority
    virtual uint32_t getRenderOrder() {return m_renderOrder;}
    //Sets object's render order priority
    virtual void setRenderOrder(const uint32_t _newRO) {m_renderOrder = _newRO;}

    //returns material instance (ideally not overriden)
    virtual SPTR<MaterialInstance> getMatInst() {return m_materialInstance;}
    //sets new material instance (ideally not overriden)
    virtual void setMatInst(const SPTR<MaterialInstance> _newMI) {m_materialInstance = _newMI;}
    
    //Render-related methods
    virtual std::vector<SPTR<Vertex>> getVertices() const {return m_vertices;}
    virtual size_t getVertexCount() const {return m_vertices.size();}
    virtual std::vector<size_t> getIndices() const {return m_indices;}

    //todo: hide to protected after IndexBuffer::addRenderObject replaced with RendererBatch class functionality
    virtual void adjustIndices(size_t _adjustment) = 0; 
    
protected:

    //Render-related methods (implemented and used by heir classes)
    virtual void addVertex(SPTR<Vertex> _vertex) = 0;
    virtual void addVertex(Vertex&& _vertex) = 0;
    bool isDirty() const { return m_isDirty; }
    void setIsDirty(const bool _isDirty) { m_isDirty = _isDirty; }

    void onTransformChange() override {setIsDirty(true); Node::onTransformChange(); }
    
private:
    
    bool m_isDirty = true; // set to "false" after being put in batch, reset to "true" after render parameters change
    uint32_t m_renderOrder = 0;
    SPTR<MaterialInstance> m_materialInstance = nullptr;

    std::vector<SPTR<Vertex>> m_vertices;
    std::vector<size_t> m_indices;
};
