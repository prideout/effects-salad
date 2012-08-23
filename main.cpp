#include "lib/pez/pez.h"

#include "common/camera.h"
#include "common/demoContext.h"
#include "common/init.h"
#include "common/programs.h"
#include "common/surface.h"
#include "fx/all.h"
#include "jsoncpp/json.h"
#include <iostream>

DemoContext* context;
Quads quads;

void PezInitialize()
{
    context = DemoContext::SetCurrent(DemoContext::New());
    PezConfig cfg = PezGetConfig();
    // add our shader path
    pezSwAddPath("", ".glsl");

    context->mainCam.eye.z = 5;
    context->mainCam.aspect= cfg.Width / cfg.Height;

    // initialize FX

    // XXX this seems busted:
    // it's important to re-initialize after context initialization
    // so the drawable gets the current context
    quads = Quads();
    quads.Init();
}

PezConfig PezGetConfig()
{
    PezConfig config;
    config.Title = __FILE__;
    config.Width = 2560/4;
    config.Height = 1600/4;
    config.Multisampling = false;
    config.VerticalSync = true;
    return config;
}

void PezHandleMouse(int x, int y, int action)
{
    if (action == PEZ_DOWN) {
    } else if (action == PEZ_UP) {
    }
}

void PezRender()
{
    PezConfig cfg = PezGetConfig();
    glViewport(0, 0, cfg.Width, cfg.Height);
    glClearColor(0,0,0,1);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    // render active effects
    quads.Draw();
}

float t;
void PezUpdate(float seconds)
{
    t += seconds;
    context->mainCam.eye.x += .1*cos(t*2); 

    // update active effects
    quads.Update();
}
