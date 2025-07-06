#include "../Public/VertexAO.h"

#include "BaseClasses/Public/VertexAttributes.h"
#include "Utilities/Public/GLErrorCatcher.h"

VertexAO::VertexAO()
{
    GLCall(glGenVertexArrays(1, &m_rendererId));
}

VertexAO::~VertexAO()
{
    GLCall(glDeleteVertexArrays(1, &m_rendererId));
}

void VertexAO::bind() const
{
    GLCall(glBindVertexArray(m_rendererId));
}

void VertexAO::unbind() const
{
    GLCall(glBindVertexArray(0));
}

void VertexAO::clear()
{
    m_aBuffer.clear();
    m_indicesCount = 0;
    unbind();
    GLCall(glDeleteVertexArrays(1, &m_rendererId));
    GLCall(glGenVertexArrays(1, &m_rendererId));
}