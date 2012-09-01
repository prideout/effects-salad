#pragma once

#include "common/camera.h"
#include "common/effect.h"
#include "common/init.h"
#include "common/normalField.h"


// simple effect used to test framework features

class Ground : public Effect {
    Vao _ground;
    NormalField _normals;

public:

    Ground() : Effect() 
    { 
    }
   
    virtual void Init();

    virtual void Update();

    virtual void Draw();
};

