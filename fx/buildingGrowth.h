#pragma once

#include "common/effect.h"
#include "common/sketchScene.h"

class BuildingGrowth : public Effect {
public:
    BuildingGrowth();
    virtual ~BuildingGrowth();
    virtual void Init();
    virtual void Update();
    virtual void Draw();
private:
    sketch::Scene _sketch;
};
