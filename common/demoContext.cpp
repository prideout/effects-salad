#include "demoContext.h"

#include "fx/quads.h"


DemoContext* DemoContext::_current(NULL);

DemoContext::DemoContext() : clearColor(0,0,0,1) {
}

void 
DemoContext::Init() {
    quads = new Quads();
    quads->Init();
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
    quads->Draw();

}

void 
DemoContext::Update(float seconds) {
    t += seconds;
    mainCam.eye.x += .1*cos(t*2); 

    // update active effects
    quads->Update();
}


