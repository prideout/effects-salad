#pragma once

#include "common/effect.h"
#include "common/texture.h"
#include "glm/glm.hpp"

// Fills the viewport with a solid color.
// Normally you should use glClear instead,
// but this is useful for cropped viewports.
// TODO extend this to apply texturing ability.
class Background : public Effect {
    glm::vec4 _color;
    GLuint _vao;
public:
    Background(glm::vec4 color) : Effect(), _color(color) {
    }
    virtual ~Background() {}
    virtual void Init();
    virtual void Update();
    virtual void Draw();
};
