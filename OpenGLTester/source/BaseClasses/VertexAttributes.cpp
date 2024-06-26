#include "VertexAttributes.h"

VertexAttributes::VertexAttributes()
    :
    m_stride(0)
{
}

void VertexAttributes::addAttribute(VertexBufferElement _inVBElement)
{
    m_vertexAttributes.push_back(_inVBElement); //adding new element to the vertex attributes list
    m_stride += getTypeSize(_inVBElement.type) * static_cast<GLsizei>(_inVBElement.count); //modifying stride size - the size of record about vertex attributes
}

GLsizei VertexAttributes::getTypeSize(GLenum _type) const
{
    switch (_type)
    {
        case(GL_FLOAT):         return sizeof(float);
        case(GL_UNSIGNED_INT):  return sizeof(GLuint);
        case(GL_INT):           return sizeof(GLint);
        case(GL_UNSIGNED_BYTE): return sizeof(unsigned char);
        case(GL_BYTE):          return sizeof(char);
        
        default:
            {
                std::cout << "[VertexAttributes::getTypeSize] Tried to get unsupported type size!" << std::endl;
                return 0;
            }
    }
}
