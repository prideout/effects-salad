#pragma once

#include "glm/glm.hpp"
#include "pez/pez.h"

//
// A general surface / frame-buffer-object class for screen-space effects
//
class Surface {
public:
    GLsizei width;
    GLsizei height;
    glm::vec4 clearColor;
    GLuint texture;
    GLuint depthTexture;
    GLuint normalsTexture;
    GLuint positionsTexture;
    GLuint fbo;

    Surface();

    void Init();

    enum {
        ColorFlag   = 1 << 0,
        DepthFlag   = 1 << 1,
        NormalsFlag = 1 << 2,
        PositionsFlag = 1 << 3,
    };

    typedef unsigned int Mask;

    void Init(glm::ivec2 size,
              GLenum internalFormat,
              GLenum format,
              GLenum type,
              GLenum filter,
              Mask flags,
              Surface* depthPeer = 0);

    void Bind();
    void Unbind();
    void Verify();

    float GetAspect() const { return (float) width / height; }
};


