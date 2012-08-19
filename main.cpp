#include "lib/pez/pez.h"

#include "common/init.h"
#include "common/programs.h"
#include "common/surface.h"

#include "jsoncpp/json.h"
#include <iostream>

void PezInitialize()
{
    // add our shader path
    pezSwAddPath("", ".glsl");

    printf("Running!\n");
    Programs& progs = Programs::GetInstance();
    progs.Load("Default.Simple");
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
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void PezUpdate(float seconds)
{
 
}
