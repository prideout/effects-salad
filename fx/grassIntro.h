#pragma once

#include "common/camera.h"
#include "common/curve.h"
#include "common/effect.h"
#include "common/init.h"
#include "common/instancer.h"
#include "common/normalField.h"
#include "common/surface.h"
#include "common/texture.h"
#include "common/tube.h"
#include "common/quad.h"

#include "fx/fireFlies.h"
#include "fx/fullscreen.h"
#include "fx/ground.h"
#include "fx/milkyway.h"
#include "fx/tree.h"


class GrassIntro : public Effect {
    Surface _surface;
    
    Quad _quad;
    Tube _tube;
    FireFlies _fireFlies;
    std::vector<Tube> _tubes;
    AnimCurve<glm::vec3> cameraPoints;
    Milkyway _milkyway;

public:
    // hold a pointer to ground so the video memory is
    // shared. This feels so wrong, our scene graph should
    // allow for this in a better way
    Ground* _ground;
    Fullscreen* fullscreen;

    GrassIntro() : Effect(), _ground(NULL), fullscreen(NULL) {}
    virtual ~GrassIntro() {} 
   
    virtual void Init();

    virtual void Update();

    virtual void Draw();
};

