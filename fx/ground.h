#pragma once

#include "common/camera.h"
#include "common/effect.h"
#include "common/init.h"
#include "common/normalField.h"


// simple effect used to test framework features

class Ground : public Effect {
    Vao _ground;
    Vao _grass;
    NormalField _normals;

public:

    float brightness;
    float vibration;

    Ground() : Effect(), brightness(1.0f), vibration(0.0f)
    { 
    }
   
    virtual void Init();

    virtual void Update();

    virtual void Draw();
};

