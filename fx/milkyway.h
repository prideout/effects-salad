#pragma once

#include "common/camera.h"
#include "common/effect.h"
#include "common/init.h"
#include "common/normalField.h"
#include "common/quad.h"


// simple effect used to test framework features

class Milkyway : public Effect {
    Quad _skyQuad;
    Vao _stars;

public:

    float brightness;

    Milkyway() : Effect(), brightness(1.0f) 
    { 
    }
   
    virtual void Init();

    virtual void Update();

    virtual void Draw();
};

