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
#include "fx/ground.h"
#include "fx/milkyway.h"
#include "fx/tree.h"


class GrassIntro : public Effect {
    Surface _surface;
    Ground _ground;
    Quad _quad;
    Tube _tube;
    FireFlies _fireFlies;
    std::vector<Tube> _tubes;
    AnimCurve<glm::vec3> cameraPoints;
    Milkyway _milkyway;
    unsigned counter;
    Tree _tree;

public:

    GrassIntro() : Effect() {}
    virtual ~GrassIntro() {} 
   
    virtual void Init();

    virtual void Update();

    virtual void Draw();
};

