#pragma once

#include "glm/glm.hpp"
#include "pez/pez.h"

class Surface {
public:
    GLsizei width;
    GLsizei height;
    glm::vec4 clearColor;
    GLuint texture;
    GLuint depthTexture;
    GLuint fbo;

    Surface();

    void Init();

    void Init(glm::ivec2 size,
              GLenum internalFormat,
              GLenum format,
              GLenum type,
              GLenum filter,
              bool createDepth);

    void Bind();
    void Unbind();
    void Verify();

    float GetAspect() const { return (float) width / height; }
};


