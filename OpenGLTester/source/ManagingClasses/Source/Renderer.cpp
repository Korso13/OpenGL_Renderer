#include "../Public/Renderer.h"
#include "../Public/VertexAO.h"
#include "../Public/ShaderMachine.h"
#include "BaseClasses/Public/IndexBuffer.h"
#include "Utilities/Public/GLErrorCatcher.h"
#include "BaseClasses/Public/RendererBatch.h"

#ifdef SINGLERENDER
Renderer* Renderer::m_instance = nullptr;
#endif

void Renderer::draw(VertexAO* _vertexArray, const ShaderType _shaderType, GLint renderPrimitiveType) const
{
    if(!_vertexArray)
        return;
    
    if(!ShaderMachine::get()->setShader(_shaderType))
    {
        std::cout << "Renderer::draw error - shader "
                    << std::to_string(static_cast<int>(_shaderType))
                    << " not found!"
                    << std::endl;
        return;
    }
    _vertexArray->bind(); //binds vertex buffer, index buffer and vertex attributes
    const int indexSize = (_vertexArray->getIndexBuffers().empty()) ? (6) : (_vertexArray->getIndexBuffers()[0]->getCount()); //precaution against vertexAO without index buffer
    GLCall(glDrawElements(renderPrimitiveType, indexSize, GL_UNSIGNED_INT, 0));
}

void Renderer::render()
{
    clear(); //clearing buffer

    //todo:collect batches
    
    //rendering collected batches and clearing queue
    for(const auto& batch : m_currentBatchQueue)
    {
        drawBatch(batch);
    }
    m_currentBatchQueue.clear();
}

void Renderer::clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::drawBatch(SPTR<RendererBatch> batch)
{
    //todo: batch render logic
}
