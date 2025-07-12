#include "../Public/Texture.h"
#include "Utilities/Public/GLErrorCatcher.h"

Texture::Texture(const std::string& _filePath)
    :
    m_rendererId(0),
    m_buffer(nullptr),
    m_path(_filePath),
    m_size(),
    m_bytesPP(4)
{
    //prepare texture
    GLCall(glGenTextures(1, &m_rendererId));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_rendererId));

    //load image
    stbi_set_flip_vertically_on_load(1);
    m_buffer = stbi_load(m_path.c_str(), &m_size.w, &m_size.h, &m_bytesPP, 4);

    //TODO:: outsource texture instantiation and allow setting these parameters first
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE)); //horizontal wrapping
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE)); //vertical wrapping

    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_size.w, m_size.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_buffer));
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));

    if(m_buffer)
        stbi_image_free(m_buffer);
}

Texture::Texture(const Texture& _texture)
    :
    //at the moment, trying to avoid multiple instances of the same texture
    m_rendererId(_texture.m_rendererId),
    m_buffer(_texture.m_buffer),
    m_path(_texture.m_path),
    m_size(_texture.m_size),
    m_bytesPP(4)
{
}

Texture& Texture::operator=(const Texture& _texture)
{
    if (this == &_texture)
        return *this;

    //at the moment, trying to avoid multiple instances of the same texture
    m_rendererId = _texture.m_rendererId;
    m_buffer =_texture.m_buffer;
    m_path = _texture.m_path;
    m_size = _texture.m_size;
    m_bytesPP = 4;

    return *this;
}

Texture::~Texture()
{
    GLCall(glDeleteTextures(1, &m_rendererId));
}

void Texture::bind(GLuint slot) const
{
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_rendererId));
}

void Texture::unbind() const
{
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
