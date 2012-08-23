#pragma once

#include <iostream>
#include "pez/pez.h"

class Texture {
public:
    GLuint handle;
    GLenum target;

    Texture();
    virtual ~Texture() {};

    virtual void Bind();
    virtual void GenMipmaps() = 0;

};

class BufferTexture : public Texture {
public:
    
    BufferTexture();

    void Init(GLenum format,
              unsigned sizeInBytes,
              const GLvoid* data); 

    virtual void GenMipmaps();
};


