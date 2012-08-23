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
    std::cout << vbo << std::endl;

    glTexBuffer(target, format, vbo);

    pezCheck(glGetError() == GL_NO_ERROR, "TexImage1D failed");
}

void
BufferTexture::GenMipmaps() {
    pezCheck(false, "GenMipmaps failed: BufferTexture cannot have mipmaps");
}

