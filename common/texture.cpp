#include "texture.h"


Texture::Texture() :
    handle(0)
{
}

void
Texture::Bind(int stage) {
    glActiveTexture(stage + GL_TEXTURE0);
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
BufferTexture::Init(GLenum internalFormat,
                    unsigned int sizeInBytes,
                    const GLvoid* data)
{
    glGenTextures(1, &handle);
    pezCheck(glGetError() == GL_NO_ERROR, "BufferTexture gen failed");

    glBindTexture(target, handle);
    pezCheck(glGetError() == GL_NO_ERROR, "Bind BufferTexture failed");

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(target, vbo);
    glBufferData(target, sizeInBytes, data, GL_STATIC_DRAW);
    pezCheck(glGetError() == GL_NO_ERROR, "TEXTURE_BUFFER population failed");

    glTexBuffer(target, internalFormat, vbo);
    pezCheck(glGetError() == GL_NO_ERROR, "glTexBuffer failed");
}

void
BufferTexture::Init(const FloatList& data)
{
    Init(GL_R32F, data.size() * 4, &data[0]);
}

void
BufferTexture::Init(const Vec3List& data)
{
    Init(GL_RGB32F, data.size() * 12, &data[0].x);
}

void
BufferTexture::Init(const Vec4List& data)
{
    Init(GL_RGBA32F, data.size() * 16, &data[0].x);
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
RectTexture::Init(GLenum format,
                  GLenum internalFormat,
                  unsigned int numTexels,
                  const GLvoid* data)
{
    glGenTextures(1, &handle);
    glBindTexture(target, handle);
    _width = numTexels > 1024 ? 1024 : numTexels;
    _height = (numTexels + 1023) / 1024;
    glTexImage2D(target,
                 0,
                 internalFormat,
                 _width,
                 _height,
                 0,
                 format,
                 GL_FLOAT,
                 (const GLvoid*) data);
    pezCheck(glGetError() == GL_NO_ERROR, "RectTexture::Init Float failed");
}

void
RectTexture::Init(const FloatList& data)
{
    Init(GL_RED, GL_R32F, data.size(), &data[0]);
}

void
RectTexture::Init(const Vec3List& data)
{
    Init(GL_RGB, GL_RGB32F, data.size(), &data[0]);
}

void
RectTexture::Init(const Vec4List& data)
{
    Init(GL_RGBA, GL_RGBA32F, data.size(), &data[0]);
}

void
RectTexture::GenMipmaps()
{
    pezFatal("GenMipmaps failed: RectTexture cannot have mipmaps");
}
