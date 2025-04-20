#include "../Public/Multipolygon.h"

void Multipolygon::addPolygon(SPTR<RenderPrimitive> _polygon)
{
    m_primitives.push_back(_polygon);
}
