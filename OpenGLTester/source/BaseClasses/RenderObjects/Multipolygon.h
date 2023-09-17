#pragma once
#include <PCH.h>
#include "Defines.h"

class RenderPrimitive;

class Multipolygon
{
public:
    Multipolygon() = default;

    void addPolygon(SPTR<RenderPrimitive> _polygon);
    const std::vector<SPTR<RenderPrimitive>>& getPolygons() const { return m_primitives; }

private:
    std::vector<SPTR<RenderPrimitive>> m_primitives;
};
