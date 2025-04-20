#pragma once
#include "Defines.h"
#include "Utilities/Public/GLErrorCatcher.h"

class RendererBatch;
class VertexAO;

class Renderer
{
//defines constructor depending on what type of instantiation we want
#ifdef MULTIRENDER
public:
    Renderer()
    {
        GLCall(glGetIntegerv(GL_MAX_TEXTURE_SIZE, &m_maxBatchSize));
    }
#endif
#ifdef SINGLERENDER
private:
    Renderer()
    {
        GLCall(glGetIntegerv(GL_MAX_TEXTURE_SIZE, &m_maxBatchSize));
    }
    static Renderer* m_instance;
    ~Renderer() = default; //prevents calling delete() on Renderer pointer
    
public:
    static Renderer* get()
    {
        if(!m_instance)
            m_instance = new Renderer();

        return m_instance;
    }
#endif
//end of constructor definitions
    
public:
    //todo: depricate
    void draw(VertexAO* _vertexArray, const ShaderType _shaderType, GLint renderPrimitiveType = GL_TRIANGLES) const;
    
    //Main rendering function - assembles all batches and renders them.
    //Do not call yourself! Should only be called from main program loop (todo: consider making private, adding friendly iGame and impl to call it)
    void render();
    void clear() const;

private:

    void drawBatch(SPTR<RendererBatch> batch);
    
private:

    GLint m_maxBatchSize = 1;
    std::vector<SPTR<RendererBatch>> m_currentBatchQueue;
};
