

#pragma once

#include "glm/glm.hpp"

class Surface {
public:
    
    GLsizei width;
    GLsizei height;
    glm::vec4 clearColor;
    glm::mat4 modelview;
    glm::mat4 projection;
    GLuint texture;
    GLuint depth;
    GLuint fbo;

    
    Surface() {};
    ~Surface() {};

    void Clear();
    void Verify();
};


