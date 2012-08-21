#pragma once

#include <iostream>
#include "pez/pez.h"

class Texture {
public:
    GLuint handle;

    Texture();
    virtual ~Texture() {};

    virtual void Bind() = 0;
    virtual void GenMipmaps() = 0;

};

class Texture1D : public Texture {
public:
    
    Texture1D();

    void Init(GLenum target,
              GLint level,
              GLint internalformat, 
              GLsizei width,
              GLint border,
              GLenum format,
              GLenum type,
              const GLvoid* data); 

    void Init(GLint internalformat, 
              GLsizei width,
              GLenum format,
              GLenum type,
              const GLvoid* data);

    virtual void Bind();
    virtual void GenMipmaps();
};


