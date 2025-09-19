#include "../Public/VertexAO.h"

#include "Utilities/Public/GLErrorCatcher.h"
#include "Utilities/Public/ImG_AutoDrawers.h"

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
    m_indicesCount = 0;
    unbind();
    GLCall(glDeleteVertexArrays(1, &m_rendererId));
    GLCall(glGenVertexArrays(1, &m_rendererId));
}

bool VertexAO::onGui(const std::string& _name)
{
    bool result = false;
    result = result || AutoDrawers::DrawClassVariables("VertexAO",
        NamedVar<size_t*>{"Indices in AO", &m_indicesCount}
    );
    return result;
}
