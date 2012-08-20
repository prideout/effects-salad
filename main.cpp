#include "lib/pez/pez.h"

#include "common/camera.h"
#include "common/init.h"
#include "common/programs.h"
#include "common/surface.h"
#include "common/quad.h"

#include "jsoncpp/json.h"
#include <iostream>

Quad quad;
PerspCamera cam;

void PezInitialize()
{
    PezConfig cfg = PezGetConfig();
    // add our shader path
    pezSwAddPath("", ".glsl");
    quad.Init();

    cam.eye.z = 5;
    cam.aspect= cfg.Width / cfg.Height;

    printf("Running!\n");
    Programs& progs = Programs::GetInstance();
    glUseProgram(progs.Load("Default.Simple"));
    pezCheck(glGetError() == GL_NO_ERROR, "compile failed");
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
    cam.Bind(quad.modelMat);
    quad.Draw();
    pezCheck(glGetError() == GL_NO_ERROR, "draw failed");
}

float t;
void PezUpdate(float seconds)
{
    t += seconds;
    cam.eye.x += .1*cos(t*2); 
}
