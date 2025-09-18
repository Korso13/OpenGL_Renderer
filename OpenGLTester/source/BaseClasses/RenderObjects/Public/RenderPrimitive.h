#pragma once
#include <pch.h>
#include <Defines.h>
#include "RenderObject.h"

class RenderPrimitive : public RenderObject
{
public:
    explicit RenderPrimitive(const std::string& _name,
        std::vector<SPTR<Vertex>> _vertices,
        std::vector<size_t>&& _indices);
    
    size_t getVertexCount() const override {return RenderObject::getVertexCount();}

    
private:
};
