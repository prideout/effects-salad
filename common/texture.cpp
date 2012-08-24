#include "texture.h"


Texture::Texture() :
    handle(0)
{
}

void
Texture::Bind() {
    glBindTexture(target, handle);
}


//
// Buffer Texture
//

BufferTexture::BufferTexture() :
    Texture()
{
    target = GL_TEXTURE_BUFFER;
}



void
BufferTexture::Init(
          GLenum format,
          unsigned sizeInBytes,
          const GLvoid* data) {
    glGenTextures(1, &handle);
    pezCheck(glGetError() == GL_NO_ERROR, "BufferTexture gen failed");

    glBindTexture(target, handle);
    pezCheck(glGetError() == GL_NO_ERROR, "Bind BufferTexture failed");

    pezCheck(glGetError() == GL_NO_ERROR, "Set TexParam failed");

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeInBytes, data, GL_STATIC_DRAW);

    glTexBuffer(target, format, vbo);

    pezCheck(glGetError() == GL_NO_ERROR, "TexImage1D failed");
}

void
BufferTexture::GenMipmaps() {
    pezFatal("GenMipmaps failed: BufferTexture cannot have mipmaps");
}

//
// Rect Texture
//

RectTexture::RectTexture() :
    Texture()
{
    target = GL_TEXTURE_RECTANGLE;
}

void
RectTexture::Init(const FloatList& data)
{
    glGenTextures(1, &handle);
    glBindTexture(target, handle);
    GLenum internalFormat = GL_R32F;
    GLenum format = GL_RED;
    _width = data.size() > 1024 ? 1024 : data.size();
    _height = (data.size() + 1023) / 1024;
    glTexImage2D(target,
                 0,
                 internalFormat,
                 _width,
                 _height,
                 0,
                 format,
                 GL_FLOAT,
                 (const GLvoid*) &(data[0]));
    pezCheck(glGetError() == GL_NO_ERROR, "RectTexture::Init Float failed");
}

void
RectTexture::Init(const Vec3List& data)
{
    glGenTextures(1, &handle);
    glBindTexture(target, handle);
    GLenum internalFormat = GL_RGB32F;
    GLenum format = GL_RGB;
    _width = data.size() > 1024 ? 1024 : data.size();
    _height = (data.size() + 1023) / 1024;
    glTexImage2D(target,
                 0,
                 internalFormat,
                 _width,
                 _height,
                 0,
                 format,
                 GL_FLOAT,
                 (const GLvoid*) &(data[0]));
    pezCheck(glGetError() == GL_NO_ERROR, "RectTexture::Init Vec3 failed");
}

void
RectTexture::Init(const Vec4List& data)
{
    glGenTextures(1, &handle);
    glBindTexture(target, handle);
    GLenum internalFormat = GL_RGBA32F;
    GLenum format = GL_RGBA;
    _width = data.size() > 1024 ? 1024 : data.size();
    _height = (data.size() + 1023) / 1024;
    glTexImage2D(target,
                 0,
                 internalFormat,
                 _width,
                 _height,
                 0,
                 format,
                 GL_FLOAT,
                 (const GLvoid*) &(data[0]));
    pezCheck(glGetError() == GL_NO_ERROR, "RectTexture::Init Vec4 failed");
}

void
RectTexture::GenMipmaps()
{
    pezFatal("GenMipmaps failed: RectTexture cannot have mipmaps");
}
