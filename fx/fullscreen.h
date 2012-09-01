#pragma once

#include "common/effect.h"
#include "common/texture.h"
#include "common/surface.h"
#include "glm/glm.hpp"

class Fullscreen : public Effect {
public:

    glm::vec4 solidColor;

    enum {
        SolidColorFlag = 1 << 0,
        VignetteFlag   = 1 << 1,
        ScanLinesFlag  = 1 << 2,
    };

    typedef unsigned int Mask;

    Fullscreen(Mask mask, Effect* child = 0) :
        Effect(), _mask(mask), _child(child) {
    }

    virtual ~Fullscreen() {}
    virtual void Init();
    virtual void Update();
    virtual void Draw();

private:

    Surface _surface;
    GLuint _vao;
    Mask _mask;
    Effect* _child;
};
