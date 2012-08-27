#include "viewport.h"

Viewport::Viewport() : x(0), y(0) {
    PezConfig cfg = PezGetConfig();
    width = cfg.Width;
    height = cfg.Height;
}

Viewport::Viewport(GLsizei width, GLsizei height) : 
    x(0), 
    y(0),
    width(width),
    height(height) { 
}


Viewport::Viewport(GLint x, GLint y, GLsizei width, GLsizei height) : 
    x(x), 
    y(y),
    width(width),
    height(height) { 
}

void 
Viewport::Bind() {
    glViewport(x, y, width, height);
}


