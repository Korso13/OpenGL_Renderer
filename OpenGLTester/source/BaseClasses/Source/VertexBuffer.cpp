#include "../Public/VertexBuffer.h"
#include "Utilities/Public/GLErrorCatcher.h"
#include "Utilities/Public/ImG_AutoDrawers.h"
#include "BaseClasses/RenderObjects/Public/RenderObject.h"

VertexBuffer::VertexBuffer(const void* _data, const unsigned int _size, GLenum _memUsageType)
:
EngineInternal("VertexBuffer")
{
    GLCall(glGenBuffers(1, &m_rendererId));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererId));
    GLCall(glBufferData(GL_ARRAY_BUFFER, _size, _data, _memUsageType));
}

VertexBuffer::VertexBuffer(const unsigned int _size)
:
EngineInternal("VertexBuffer")
{
    GLCall(glGenBuffers(1, &m_rendererId));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererId));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * _size, nullptr, GL_DYNAMIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_rendererId));
}

void VertexBuffer::addRenderObject(const std::string& _name, const SPTR<RenderObject>& _objRef)
{
    m_renderPool.emplace(std::string(_name), _objRef);
    m_verticesToDraw += _objRef->getVertexCount();
    for(auto& vertex : _objRef->getVertices())
    {
        m_verticesPool.push_back(*vertex);
    }
}

void VertexBuffer::removeRenderObject(const std::string& _name)
{
    if(!m_renderPool.contains(_name))
        return;

    m_verticesToDraw -= m_renderPool[_name].lock()->getVertexCount();
    m_renderPool.erase(_name);
    m_verticesPool.clear();
    updateVerticesPool();
}

void VertexBuffer::bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererId));
    
    if(!m_verticesPool.empty())
    {
        if (m_verticesPool.size() != m_verticesToDraw)
        {
            std::cerr << "[VertexBuffer::bind()] vertices pool and verticesToDraw size mismatch!\n";
            return;
        }
        // GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * m_verticesToDraw, m_verticesPool.data()));

        //todo: checking for possible fix
        Vertex* vertices = new Vertex[m_verticesToDraw];
        size_t vertex_counter = 0;

        static auto vec3Str = [](const vec3& _vec) -> std::string
        {
            std::string out = "{ x: ";
            out += std::to_string(_vec.x);
            out += ", y: ";
            out += std::to_string(_vec.y);
            out += ", z: ";
            out += std::to_string(_vec.z);
            out += "}";
            return out;
        };
        
        //std::cout << "====================================================\n";

        //for(auto& vert : m_verticesPool)
        for(auto& [obj_name, obj_weak] : m_renderPool)
        {
            if(obj_weak.expired()) continue;
            for(auto& vert : obj_weak.lock()->getVertices())
            {
                vertices[vertex_counter] = *vert; //todo: if we keep this code - abolish copying!
                // std::cout << "[" << vertex_counter << "]\n";
                // std::cout << "Vertex original idx: " << vert.vertexIndex << " - vertex copied: " << vertices[vertex_counter].vertexIndex <<"\n";
                // std::cout << "Vertex original coord: " << vec3Str(vert.position) << " - vertex copied coord: " << vec3Str(vertices[vertex_counter].position) <<"\n";
                vertex_counter++;
            }
        }
        // std::cout << "-|-\n";
        // for (size_t i = 0; i < vertex_counter; ++i)
        // {
        //     std::cout << "Vertex " << vertices[i].vertexIndex <<"\n";
        // }
        // std::cout << "====================================================\n";
        GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * m_verticesToDraw, vertices));
    }
}

void VertexBuffer::unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void VertexBuffer::clear()
{
    m_renderPool.clear();
    m_verticesPool.clear();
    m_verticesToDraw = 0;
}

bool VertexBuffer::onGui(const std::string& _name)
{
    bool result = false;
    result = result || AutoDrawers::DrawClassVariables("VertexBuffer",
        NamedVar<size_t*>{"Vertices to draw", &m_verticesToDraw},
        NamedContainer<std::unordered_map<std::string, WPTR<RenderObject>>*>{"Render pool", &m_renderPool},
        NamedContainer<std::vector<Vertex>*>{"Vertices", &m_verticesPool},
        SubMenu{_name, SUB_MENU_CALL(return result = result || EngineInternal::onGui(_name);)}
    );
    if(result)
        updateVerticesPool();
    return result;
}

void VertexBuffer::updateVerticesPool()
{
    m_verticesPool.clear();
    std::erase_if(m_renderPool, [](const  std::pair<std::string, WPTR<RenderObject>>& _renderPoolPair)
    {
        return _renderPoolPair.second.expired();
    });
    
    for (auto& [name, objWptr] : m_renderPool)
    {
        if (objWptr.expired())
            continue;
        
        auto objRef = objWptr.lock();
        for(auto& vertex : objRef->getVertices())
        {
            m_verticesPool.push_back(*vertex); 
        }
    }
}
