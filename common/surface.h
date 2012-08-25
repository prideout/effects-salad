#pragma once

#include "glm/glm.hpp"
#include "pez/pez.h"

class Surface {
public:
    GLsizei width;
    GLsizei height;
    glm::vec4 clearColor;
    GLuint texture;
    GLuint depth;
    GLuint fbo;

    Surface();

    void Init();
    void Bind();
    void Unbind();
    void Verify();
};


