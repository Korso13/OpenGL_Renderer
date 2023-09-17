#pragma once
#include <PCH.h>
#include <Defines.h>
#include "RenderObject.h"

class RenderPrimitive : public RenderObject
{
public:
    RenderPrimitive() = default;

    std::vector<SPTR<Vertex>> getVertexes() override {return m_polygonData;}
    size_t getVertexCount() const override {return m_polygonData.size();}
    
    void addVertex(SPTR<Vertex> _vertex) override;
    void addVertex(Vertex&& _vertex) override;
    
private:
    std::vector<SPTR<Vertex>> m_polygonData;
};
