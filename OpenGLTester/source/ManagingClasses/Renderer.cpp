#include "Renderer.h"
#include "VertexAO.h"
#include "ShaderMachine.h"
#include "BaseClasses/IndexBuffer.h"
#include "Utilities/GLErrorCatcher.h"

#ifdef SINGLERENDER
Renderer* Renderer::m_instance = nullptr;
#endif

void Renderer::draw(VertexAO* _vertexArray, const ShaderType _shaderType, GLint renderPrimitiveType) const
{
    if(!_vertexArray)
        return;
    
    ShaderMachine::get()->setShader(_shaderType);
    _vertexArray->bind(); //binds vertex buffer, index buffer and vertex attributes
    const int indexSize = (_vertexArray->getIndexBuffers().empty()) ? (6) : (_vertexArray->getIndexBuffers()[0]->getCount()); //precaution against vertexAO without index buffer
    GLCall(glDrawElements(renderPrimitiveType, indexSize, GL_UNSIGNED_INT, 0));
}

void Renderer::clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}
