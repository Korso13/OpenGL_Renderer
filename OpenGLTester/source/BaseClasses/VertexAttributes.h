#pragma once
#include <PCH.h>
#include <vector>

struct VertexBufferElement
{
    GLenum type;
    unsigned int count;
    GLboolean isNotNormalized;
};

class VertexAttributes
{
public:

    VertexAttributes();

    /*
     Use this to add elements to vertex layout of attributes, such as coordinates record, normals record, color record and etc.
     */
    void addAttribute(VertexBufferElement _inVBElement);

    GLsizei getStride() const {return m_stride;}
    unsigned int getTypeSize(GLenum _type) const;
    const std::vector<VertexBufferElement> getVertexAttributes() const {return m_vertexAttributes;}

private:

    std::vector<VertexBufferElement> m_vertexAttributes;
    GLsizei m_stride;
};
