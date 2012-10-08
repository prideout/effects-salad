#pragma once

#include "common/effect.h"
#include "common/sketchPlayback.h"
#include "common/sketchScene.h"
#include "common/vao.h"
#include "common/camera.h"
#include "glm/glm.hpp"

class GridCity : public Effect {
public:
    GridCity();
    virtual ~GridCity();
    virtual void Init();
    virtual void Update();
    virtual void Draw();
private:
    sketch::Tessellator* _tess;
    sketch::Playback* _player;
    Vao _terrainVao;
    PerspCamera _camera;
};
