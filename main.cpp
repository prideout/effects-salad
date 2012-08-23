#include "lib/pez/pez.h"

#include "common/camera.h"
#include "common/demoContext.h"
#include "common/init.h"
#include "common/instancer.h"
#include "common/programs.h"
#include "common/surface.h"
#include "common/quad.h"
#include "common/texture.h"

#include "jsoncpp/json.h"
#include <iostream>

DemoContext* context;
Instancer manyQuads;
Quad quad;
BufferTexture tex;

void PezInitialize()
{
    context = DemoContext::SetCurrent(DemoContext::New());
    PezConfig cfg = PezGetConfig();
    // add our shader path
    pezSwAddPath("", ".glsl");
    quad.Init();
    manyQuads.vao = quad.vao;
    manyQuads.Init();

    context->mainCam.eye.z = 5;
    context->mainCam.aspect= cfg.Width / cfg.Height;


    printf("Running!\n");
    Programs& progs = Programs::GetInstance();
    glUseProgram(progs.Load("Default.Simple"));
    glUseProgram(progs.Load("Default.Instanced", "Default.Simple.FS", "Default.Instanced.VS"));
    pezCheck(glGetError() == GL_NO_ERROR, "compile failed");

    float x[] = {1.5,-1.5,200,300,40,40,40,40,40,40,40,40,40,40,40,40};
    tex.Init(GL_R32F, 16*sizeof(float), x);
    pezCheck(glGetError() == GL_NO_ERROR, "Texture init failed");
    glUniform1i(u("Offsets"), 0);
    pezCheck(glGetError() == GL_NO_ERROR, "Texture init failed1");
    glActiveTexture(GL_TEXTURE0);
    pezCheck(glGetError() == GL_NO_ERROR, "Texture init failed2");
    tex.Bind();
    pezCheck(glGetError() == GL_NO_ERROR, "Texture bind failed3");
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
    context->mainCam.Bind(quad.modelMat);
    //quad.Draw();
    manyQuads.Draw();
    pezCheck(glGetError() == GL_NO_ERROR, "draw failed");
}

float t;
void PezUpdate(float seconds)
{
    quad.Update();
    manyQuads.Update();
    t += seconds;
    context->mainCam.eye.x += .1*cos(t*2); 
}
