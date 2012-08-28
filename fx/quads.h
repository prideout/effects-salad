#pragma once

#include "common/camera.h"
#include "common/effect.h"
#include "common/init.h"
#include "common/instancer.h"
#include "common/surface.h"
#include "common/texture.h"
#include "common/quad.h"



// simple effect used to test framework features

class Quads : public Effect {
public:
    Quad quad;
    Instancer manyQuads;
    BufferTexture tex;
    Surface surface;
    PerspCamera surfaceCam;

    Quads() : Effect() {
    }
    virtual ~Quads() {} ;
   
    virtual void Init();

    virtual void Update();

    virtual void Draw();
};

