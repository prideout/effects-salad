#pragma once

#include "glm/glm.hpp"
#include "pez/pez.h"

#include "camera.h"
#include "drawable.h"


class DemoContext {
    static const DemoContext* _current;

    DemoContext() {}
public:

    DrawableList drawables;
    PerspCamera mainCam;


    //
    // Tight control of context instances
    //
    static DemoContext*
    New() {
        return new DemoContext();
    }

    static const DemoContext*
    GetCurrent() { 
        return _current; 
    }

    static DemoContext* 
    SetCurrent(DemoContext* cur) {
        _current = cur;
        return cur;
    }

};

