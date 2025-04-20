#pragma once
#include <pch.h>
#include <Defines.h>
#include "ManagingClasses/Public/Renderer.h"

class Texture
{
public:
    Texture(const std::string _filePath);
    ~Texture();

    void bind(GLuint slot = 0) const;
    void unbind() const;

    ivec2 getSize() const {return m_size;}
    
private:

    GLuint m_rendererId;
    unsigned char* m_buffer;
    
    std::string m_path;
    ivec2 m_size;
    int m_bytesPP;
};
