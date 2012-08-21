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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    */
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage1D(target,
                 level,
                 internalformat,
                 width,
                 border,
                 format,
                 type, 
                 data);
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
    glBindTexture(GL_TEXTURE_1D, handle);
}

void
Texture1D::GenMipmaps() {
    glGenerateMipmap(GL_TEXTURE_1D);
}

