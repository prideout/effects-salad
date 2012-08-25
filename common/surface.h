

#pragma once

#include "glm/glm.hpp"

class Surface {
public:
    
    GLsizei width;
    GLsizei height;
    glm::vec4 clearColor;
    GLuint texture;
    GLuint depth;
    GLuint fbo;

    
    Surface() {};
    ~Surface() {};

    void Clear();
    void Verify();
};


