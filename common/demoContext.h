#pragma once

#include "glm/glm.hpp"
#include "pez/pez.h"


#include "audio.h"
#include "camera.h"
#include "drawable.h"
#include "effect.h"
#include "light.h"
#include "programs.h"
#include "viewport.h"


//
// "ShotContext" may be a better name for this class, it provides the current
// state for all currently rendering effects. The duration and elapsed time are
// relative to the current shot. The Audio object is the one exception that is
// globally shared.
//
class DemoContext {
    static DemoContext* _current;
    DemoContext(const std::string& shot);

public:

    float duration;
    float elapsedTime;
    float startTime;
    static float totalTime;
    std::string name;

    Audio* const audio;

    DrawableList drawables;
    LightList lights;

    PerspCamera mainCam;
    Viewport viewport;
    glm::vec4 clearColor;
    float deltaTime;

    void Init();
    void Render();
    void Update(float seconds);

    //
    // Tight control of context instances
    //
    static DemoContext*
    New(const std::string& shot) {
        return new DemoContext(shot);
    }

    static DemoContext*
    GetCurrent() { 
        return _current; 
    }

    static DemoContext* 
    SetCurrent(DemoContext* cur);
};

