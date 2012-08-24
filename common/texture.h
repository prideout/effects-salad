#pragma once

#include <iostream>
#include <vector>

#include "pez/pez.h"
#include "common/typedefs.h"

class Texture {
public:
    GLuint handle;
    GLenum target;

    Texture();
    virtual ~Texture() {};

    virtual void Bind(int stage = 0);
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

class RectTexture : public Texture {
    int _width;
    int _height;
public:
    RectTexture();
    void Init(const FloatList& data);
    void Init(const Vec3List& data);
    void Init(const Vec4List& data);
    virtual void GenMipmaps();
};
