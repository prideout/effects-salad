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
        BlendFlag      = 1 << 5,
    };

    typedef unsigned int Mask;

    Fullscreen(Mask mask, Effect* child = 0);
    Fullscreen(std::string customProgram, Mask mask = 0);
    virtual ~Fullscreen() {}

    void AddChild(Effect* child);
    void ShareDepth(Fullscreen* depthPeer);

    virtual void Init();
    virtual void Update();
    virtual void Draw();

private:

    Surface _surface;
    Vao _emptyVao;
    Mask _mask;
    std::string _customProgram;
    EffectList _children;
    Fullscreen* _depthPeer;
};
