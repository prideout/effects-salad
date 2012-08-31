#pragma once

#include "common/vao.h"
#include "common/texture.h"
#include "common/effect.h"

class Buildings : public Effect {
    Vao _buildingVao;
    BufferTexture _centroidTexture;
    int _totalTetCount;
    int _boundaryTetCount;
public:
    Buildings() : Effect() {}
    virtual ~Buildings() {} 
    virtual void Init();
    virtual void Update();
    virtual void Draw();
};
