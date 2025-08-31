#pragma once
#include "Defines.h"
#include "Utilities/Public/GLErrorCatcher.h"

class Camera;
class MaterialInstance;
class RenderObject;
class Node;
class RendererBatch;
class VertexAO;

//todo: add RenderPipeline interface class with render() and checkNodeForRender() methods. Use interface impls as switchable render pipelines (Strategy pattern)

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
    //todo: depricated. Remove after Batch rendering is done
    void draw(const VertexAO* _vertexArray, const ShaderType _shaderType, GLint _renderPrimitiveType = GL_TRIANGLES) const{};
    
    //Main rendering function - assembles all batches and renders them.
    //Do not call yourself! Should only be called from main program loop (todo: consider making private, adding friendly iGame and impl to call it)
    void render();
    void clear() const;

    SPTR<Node> getRoot();
    SPTR<Camera> getCamera();
    GLint getMaxBatchSize() const {return m_maxBatchSize;}
    
    bool onGui(const std::string& _name);
    
private:

    ~Renderer() = default; //prevents calling delete() on Renderer pointer
    void drawBatch(UPTR<RendererBatch>& _batch);
    void checkNodeForRender(const SPTR<Node>& _node);
    //updates existing batches' content and removes expired batches
    void updateRenderBatches();

private:

    GLint m_maxBatchSize = 1;
    SPTR<Node> m_rootNode = nullptr;
    SPTR<Camera> m_camera = nullptr;
    
    //Multi-layered sorted render map: distance to camera - RenderOrder - MaterialInstance(by id)
    std::map<float, std::map<uint32_t, std::unordered_map<uint32_t, std::vector<UPTR<RendererBatch>>>>> m_sortedRenderPriority;
};
