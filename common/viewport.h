#pragma once

#include "pez/pez.h"

class Viewport {
public:

    GLint x;
    GLint y;
    GLsizei width;
    GLsizei height;

    Viewport();
    Viewport(GLsizei width, GLsizei height);
    Viewport(GLint x, GLint y, GLsizei width, GLsizei height);

    // Set the GL viewport
    void Bind();
};
