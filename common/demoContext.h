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


class DemoContext {
    static DemoContext* _current;
    DemoContext(const std::string& shot);

public:

    float duration;
    float elapsedTime;
    static float totalTime;
    std::string name;

    Audio& audio;

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

