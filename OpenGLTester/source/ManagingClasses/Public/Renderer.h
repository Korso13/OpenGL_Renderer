#pragma once
#include "Defines.h"
#include "Utilities/Public/GLErrorCatcher.h"

class RenderObject;
class Node;
class RendererBatch;
class VertexAO;

class Renderer
{
//defines constructor depending on what type of instantiation we want
#ifdef MULTIRENDER
public:
    Renderer();
#endif
#ifdef SINGLERENDER
private:
    Renderer();
    static Renderer* m_instance;
public:
    static Renderer* get()
    {
        if(!m_instance) m_instance = new Renderer();
        return m_instance;
    }
#endif
//end of constructor definitions
    
public:
    //todo: depricate once batch-rendering refactoring complete
    void draw(const VertexAO* _vertexArray, const ShaderType _shaderType, GLint _renderPrimitiveType = GL_TRIANGLES) const;
    
    //Main rendering function - assembles all batches and renders them.
    //Do not call yourself! Should only be called from main program loop (todo: consider making private, adding friendly iGame and impl to call it)
    void render();
    void clear() const;

    SPTR<Node> getRoot();
    
private:

    ~Renderer() = default; //prevents calling delete() on Renderer pointer
    void drawBatch(SPTR<RendererBatch> _batch);
    void checkNodeForRender(SPTR<Node> _node);
    
private:

    GLint m_maxBatchSize = 1;
    SPTR<Node> m_rootNode = nullptr;
    std::map<float, std::map<uint32_t, SPTR<RenderObject>>> m_sortedRenderPriority;
    std::vector<SPTR<RendererBatch>> m_currentBatchQueue;
};
