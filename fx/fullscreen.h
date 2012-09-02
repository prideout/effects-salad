#pragma once

#include "common/effect.h"
#include "common/typedefs.h"
#include "common/texture.h"
#include "common/surface.h"
#include "common/vao.h"
#include "glm/glm.hpp"

class Fullscreen : public Effect {
public:

    glm::vec4 solidColor;
    glm::vec4 clearColor;

    enum {
        SolidColorFlag = 1 << 0,
        VignetteFlag   = 1 << 1,
        ScanLinesFlag  = 1 << 2,
        TeleLinesFlag  = 1 << 3,
        CopyDepthFlag  = 1 << 4,
    };

    typedef unsigned int Mask;

    Fullscreen(Mask mask, Effect* child = 0) :
        Effect(), _mask(mask) {
        clearColor = glm::vec4(0.1, 0.2, 0.4, 1);
        if (child) {
            _children.push_back(child);
        }
    }

    virtual void AddChild(Effect* child);
    virtual ~Fullscreen() {}
    virtual void Init();
    virtual void Update();
    virtual void Draw();

private:

    Surface _surface;
    Vao _emptyVao;
    Mask _mask;
    EffectList _children;
};
