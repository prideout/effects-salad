#include "texture.h"


Texture::Texture() {
}

Texture1D::Texture1D() { }

void
Texture1D::Init(GLenum target,
          GLint level,
          GLint internalformat, 
          GLsizei width,
          GLint border,
          GLenum format,
          GLenum type,
          const GLvoid* data) {

    glGenTextures(1, &handle);
    pezCheck(glGetError() == GL_NO_ERROR, "Texture gen failed");
    glBindTexture(target, handle);
    pezCheck(glGetError() == GL_NO_ERROR, "Bind Texture failed");

    /*
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    */
    pezCheck(glGetError() == GL_NO_ERROR, "Set TexParam failed");

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, width*sizeof(float), data, GL_STATIC_DRAW);
    std::cout << vbo << std::endl;

    /*
    glTexImage1D(target,
                 level,
                 internalformat,
                 width,
                 border,
                 format,
                 type, 
                 data);
    */

    glTexBuffer(target, format, vbo);

    pezCheck(glGetError() == GL_NO_ERROR, "TexImage1D failed");
}


void
Texture1D::Init(GLint internalformat, 
              GLsizei width,
              GLenum format,
              GLenum type,
              const GLvoid* data) {
    Init(GL_TEXTURE_1D, 0, internalformat, width, 0, format, type, data);
}

void
Texture1D::Bind() {
    glBindTexture(GL_TEXTURE_BUFFER, handle);
}

void
Texture1D::GenMipmaps() {
    glGenerateMipmap(GL_TEXTURE_BUFFER);
}

