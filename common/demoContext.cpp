#include "demoContext.h"

#include "typedefs.h"

#include "fx/quads.h"
#include "fx/fpsOverlay.h"

DemoContext* DemoContext::_current(NULL);
float DemoContext::totalTime = 0;

DemoContext::DemoContext(const std::string& shot) :
    duration(0),
    elapsedTime(0),
    name(shot),
    audio(Audio::Get()),
    clearColor(0,0,0,1) {
}

void 
DemoContext::Init() {
    elapsedTime = 0;
    FOR_EACH(drawable, drawables) {
        (*drawable)->Init();
    }
}

void 
DemoContext::Render() {
    viewport.Bind();
    glClearColor(clearColor.r, 
                 clearColor.g,
                 clearColor.b,
                 clearColor.a);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    // render active effects
    FOR_EACH(drawable, drawables) {
        (*drawable)->Draw();
    }

}

void 
DemoContext::Update(float seconds) {
    deltaTime = seconds;
    elapsedTime += seconds;
    mainCam.eye.x += .1*cos(elapsedTime*2); 
    mainCam.aspect = viewport.GetAspect();

    // update active effects
    FOR_EACH(drawable, drawables) {
        (*drawable)->Update();
    }
}

DemoContext*
DemoContext::SetCurrent(DemoContext* cur) {
    _current = cur;
    cur->viewport.Bind();
    return cur;
}

