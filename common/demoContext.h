#pragma once

#include "glm/glm.hpp"
#include "pez/pez.h"


#include "camera.h"
#include "drawable.h"
#include "effect.h"
#include "light.h"
#include "programs.h"
#include "viewport.h"


class DemoContext {
    static DemoContext* _current;

    float t;
    DemoContext();

public:

    DrawableList drawables;
    LightList lights;

    PerspCamera mainCam;
    Viewport viewport;
    glm::vec4 clearColor;

    void Init();
    void Render();
    void Update(float seconds);

    //
    // Tight control of context instances
    //
    static DemoContext*
    New() {
        return new DemoContext();
    }

    static DemoContext*
    GetCurrent() { 
        return _current; 
    }

    static DemoContext* 
    SetCurrent(DemoContext* cur) {
        _current = cur;
        return cur;
    }

};

