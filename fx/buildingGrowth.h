#pragma once

#include "common/effect.h"
#include "common/sketchScene.h"
#include "common/sketchTess.h"

class BuildingGrowth : public Effect {
public:
    BuildingGrowth();
    virtual ~BuildingGrowth();
    virtual void Init();
    virtual void Update();
    virtual void Draw();
private:
    sketch::Scene _sketch;
    sketch::Tessellator* _tess;
    sketch::CoplanarPath* _roof;
    Vao _vao;
};
