#include "demoContext.h"

#include "typedefs.h"

#include "fx/quads.h"


DemoContext* DemoContext::_current(NULL);

DemoContext::DemoContext() : clearColor(0,0,0,1) {
}

void 
DemoContext::Init() {
    drawables.push_back(new Quads());
    FOR_EACH(drawable, drawables) {
        (*drawable)->Init();
        cout << "init" << endl;
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
    t += seconds;
    mainCam.eye.x += .1*cos(t*2); 

    // update active effects
    FOR_EACH(drawable, drawables) {
        (*drawable)->Update();
    }
}


