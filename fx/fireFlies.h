#pragma once

#include "common/camera.h"
#include "common/curve.h"
#include "common/effect.h"
#include "common/init.h"
#include "common/instancer.h"
#include "common/surface.h"
#include "common/texture.h"
#include "common/quad.h"



class FireFlies : public Effect {
    FloatList _fliesCpu;
    Vao _fliesGpu;
    Vao _ground;
    Surface _surface;
    Quad _quad;
    Vec3List cameraPoints;
    int counter;

public:

    FireFlies() : Effect() {}
    virtual ~FireFlies() {} 
   
    virtual void Init();

    virtual void Update();

    virtual void Draw();
};

