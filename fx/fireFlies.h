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

#include "fx/ground.h"
#include "fx/milkyway.h"
#include "fx/tree.h"


class FireFlies : public Effect {
    FloatList _fliesCpu;
    Vao _fliesGpu;
    Vao _stars;
    Surface _surface;
    Ground _ground;
    Quad _quad;
    Tube _tube;
    std::vector<Tube> _tubes;
    AnimCurve<glm::vec3> cameraPoints;
    Milkyway _milkyway;
    unsigned counter;
    Tree _tree;

public:

    FireFlies() : Effect() {}
    virtual ~FireFlies() {} 
   
    virtual void Init();

    virtual void Update();

    virtual void Draw();
};

