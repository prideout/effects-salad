#pragma once

#include "common/effect.h"
#include "common/sketchup.h"

class BuildingGrowth : public Effect {
public:
    BuildingGrowth();
    virtual ~BuildingGrowth();
    virtual void Init();
    virtual void Update();
    virtual void Draw();
private:
    Sketchup::Scene _sketch;
};
