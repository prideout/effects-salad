#include "lib/pez/pez.h"
#include "common/surface.h"
#include "jsoncpp/json.h"
#include <iostream>

void PezInitialize()
{
    printf("Running!\n");
}

PezConfig PezGetConfig()
{
    PezConfig config;
    config.Title = __FILE__;
    config.Width = 2560/2;
    config.Height = 1600/2;
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
    // First draw the tube into the env map, then to the backbuffer:
    glEnable(GL_DEPTH_TEST);
}

void PezUpdate(float seconds)
{
 
}
