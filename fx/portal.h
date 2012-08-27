#pragma once

#include "common/effect.h"
#include "glm/glm.hpp"

class DemoContext;

class Portal : public Effect {
public:
    DemoContext* portalContext;
    Portal();
    virtual ~Portal() {}
    virtual void Init();
    virtual void Update();
    virtual void Draw();
};
