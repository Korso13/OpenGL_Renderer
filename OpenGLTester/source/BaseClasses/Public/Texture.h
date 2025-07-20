#pragma once
#include <pch.h>
#include <Defines.h>
#include "ManagingClasses/Public/Renderer.h"
#include "Utilities/Public/Utilities.h"

class Texture : public EngineInternal
{
public:
    Texture(const std::string& _filePath);
    Texture(const Texture& _texture);
    Texture& operator=(const Texture& _texture);
    Texture(Texture&& _texture) = delete;
    void operator=(const Texture&& _texture) = delete;
    ~Texture();
    //TODO: add constructor with color + color enum

    void bind(GLuint _slot = 0) const;
    void unbind() const;

    ivec2 getSize() const {return m_size;}
    
private:

    GLuint m_rendererId;
    unsigned char* m_buffer;
    
    std::string m_path;
    ivec2 m_size;
    int m_bytesPP;
};
