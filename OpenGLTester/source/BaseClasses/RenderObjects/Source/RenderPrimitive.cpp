#include "../Public/RenderPrimitive.h"

void RenderPrimitive::addVertex(SPTR<Vertex> _vertex)
{
    if(m_polygonData.size() >= 3)
        return;

    m_polygonData.push_back(_vertex);

}

void RenderPrimitive::addVertex(Vertex&& _vertex)
{
    if(m_polygonData.size() >= 3)
        return;
    
    m_polygonData.push_back(std::make_shared<Vertex>(_vertex));
}
