#pragma once

#include "common/effect.h"
#include "common/sketchScene.h"
#include "common/sketchTess.h"
#include "common/sketchPlayback.h"
#include "tween/CppTweener.h"

class BuildingGrowth : public Effect {
public:
    BuildingGrowth();
    virtual ~BuildingGrowth();
    virtual void Init();
    virtual void Update();
    virtual void Draw();
private:
    tween::Tweener _tween;
    sketch::Scene _sketch;
    sketch::Tessellator* _tess;
    sketch::CoplanarPath* _roof;
    sketch::Playback* _player;
    float _roofHeight;
    Vao _vao;
};
