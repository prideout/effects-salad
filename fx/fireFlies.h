#pragma once

#include "common/camera.h"
#include "common/effect.h"
#include "common/init.h"
#include "common/texture.h"
#include "common/quad.h"

class FireFlies : public Effect {
    FloatList _fliesCpu;
    Vao _fliesGpu;

public:
    float brightness;

    FireFlies() : Effect(), brightness(1.0f) {}
    virtual ~FireFlies() {} 
   
    virtual void Init();

    virtual void Update();

    virtual void Draw();
};

