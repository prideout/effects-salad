#pragma once

#include <iostream>
#include <vector>

#include "pez/pez.h"
#include "common/typedefs.h"
#include "glm/glm.hpp"

class Texture {
public:
    GLuint handle;
    GLenum target;

    Texture();
    virtual ~Texture() {};

    virtual void Bind(int stage = 0);
    virtual void Bind(int stage, std::string sampler);
    virtual void GenMipmaps() = 0;

};

class BufferTexture : public Texture {
public:
    GLuint vbo;
    GLenum drawType;
    
    BufferTexture();

    void Init(GLenum format,
              unsigned sizeInBytes,
              const GLvoid* data); 

    template <typename T>
    void Init(GLenum format, std::vector<T> data) {
        Init(format, sizeof(T) * data.size(), &data[0]);
    }

    void Init(const FloatList& data);
    void Init(const Vec3List& data);
    void Init(const Vec4List& data);

    void Rebuffer(const FloatList& data);

    virtual void GenMipmaps();
};

class RectTexture : public Texture {
    int _width;
    int _height;
public:
    RectTexture();

    void Init(GLenum format,
              GLenum internalFormat,
              unsigned int numTexels,
              const GLvoid* data);

    template <typename T>
    void Init(GLenum format, std::vector<T> data) {
        Init(format, sizeof(T) * data.size(), &data[0]);
    }

    void Init(const FloatList& data);
    void Init(const Vec3List& data);
    void Init(const Vec4List& data);
    virtual void GenMipmaps();
};

class Texture2D : public Texture {
    int _width;
    int _height;
    void _Init(GLenum format, GLenum internalFormat, const GLvoid* data);
public:
    Texture2D();
    void Init(const std::string& pngFilename);
    virtual void GenMipmaps();
    glm::ivec2 GetSize();
};
