private:

template <>
void setVertexAtributesTyped<Vertex>()
    {
        //vec3 Position;
        GLCall(glEnableVertexAttribArray(0)); //setting new vertex attributes array
        GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Position)));
        //vec4 Color; 
        GLCall(glEnableVertexAttribArray(1)); //setting new vertex attributes array
        GLCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Color)));
        //vec2 UV;
        GLCall(glEnableVertexAttribArray(2)); //setting new vertex attributes array
        GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, UV)));
        //int TextureID;
        GLCall(glEnableVertexAttribArray(3)); //setting new vertex attributes array
        GLCall(glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TextureID)));
        //unsigned int VertexIndex;
        GLCall(glEnableVertexAttribArray(4)); //setting new vertex attributes array
        GLCall(glVertexAttribPointer(4, 1, GL_UNSIGNED_INT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, VertexIndex)));
    }

public:
template<>
void addBufferTyped<Vertex>(VertexBuffer* _vertexBuffer, IndexBuffer* _indexBuffer)
    {
        m_vBuffer.push_back(_vertexBuffer);
        m_iBuffer.push_back(_indexBuffer);
    
        //binding vertexArrayObject and buffers
        bind();
        _vertexBuffer->bind(); 
        _indexBuffer->bind();

        //setting vertex attribute array
        setVertexAtributesTyped<Vertex>();
    }