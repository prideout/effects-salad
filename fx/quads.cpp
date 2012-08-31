#include "quads.h"

#include "common/demoContext.h"
#include "common/programs.h"

void Quads::Init() {
    name = "Quads";
    Effect::Init();
    Programs& progs = Programs::GetInstance();
    glUseProgram(progs.Load("Default.Simple"));

    glUseProgram(progs.Load("Tex", 
                            "Default.SimpleTex.FS", 
                            "Default.Instanced.VS"));


    glUseProgram(progs.Load("Default.Instanced", 
                            "Default.Simple.FS", 
                            "Default.Instanced.VS"));


    pezCheckGL("Quad::init compile failed");

    surface.Init();
    quad.Init();
    manyQuads.vao = quad.vao;
    manyQuads.Init();

    float x[] = {1.5,-1.5,200,300,40,40,40,40,40,40,40,40,40,40,40,40};
    
    tex.Init(GL_R32F, 16*sizeof(float), x);
    glUniform1i(u("Offsets"), 0);
    glActiveTexture(GL_TEXTURE0);
    tex.Bind();
    pezCheck(glGetError() == GL_NO_ERROR, "Texture bind failed");
}

void Quads::Update() {
    Effect::Update();
    quad.Update();
    manyQuads.Update();
}

void Quads::Draw() {
    Programs& progs = Programs::GetInstance();
    Effect::Draw();
    surface.Bind();
    surfaceCam = GetContext()->mainCam;
    surfaceCam.aspect = surface.GetAspect();
    glUseProgram(progs["Default.Instanced"]);
    surfaceCam.Bind(quad.modelMat);

    glClearColor(.0,.0,.3,1);
    glClear(GL_COLOR_BUFFER_BIT);
    manyQuads.Draw();
    surface.Unbind();
    glUseProgram(progs["Tex"]);

    glUniform1i(u("Tex"), 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, surface.texture);
    pezCheckGL("Quads::Draw Texture bind failed");

    GetContext()->viewport.Bind();
    GetContext()->mainCam.Bind(quad.modelMat);
    manyQuads.Draw();
    pezCheckGL("Quads::Draw draw failed"); 
    glBindTexture(GL_TEXTURE_2D, 0);
}

