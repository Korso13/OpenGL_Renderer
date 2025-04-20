#pragma once
#include "Defines.h"
#include <pch.h>

class RenderObject
{
public:
    virtual ~RenderObject() = default;

    virtual void addVertex(SPTR<Vertex> _vertex) = 0;
    virtual void addVertex(Vertex&& _vertex) = 0;
    virtual std::vector<SPTR<Vertex>> getVertices() = 0;
    virtual size_t getVertexCount() const = 0;
    virtual std::vector<unsigned int> getIndices() = 0;
    virtual void adjustIndices(unsigned int _adjustment) = 0;
};
