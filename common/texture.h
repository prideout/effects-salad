#pragma once

#include <iostream>
#include <vector>

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

    template <typename T>
    void Init(GLenum format, std::vector<T> data) {
        Init(format, sizeof(T) * data.size(), &data[0]);
    }
    
    virtual void GenMipmaps();
};


