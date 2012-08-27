#pragma once

#include "common/camera.h"
#include "common/effect.h"
#include "common/init.h"
#include "common/instancer.h"
#include "common/surface.h"
#include "common/texture.h"
#include "common/quad.h"



// simple effect used to test framework features

class Quads : public Effect {
public:
    Quad quad;
    Instancer manyQuads;
    BufferTexture tex;
    Surface surface;
    PerspCamera surfaceCam;

    Quads() : Effect() {
    }
    virtual ~Quads() {} ;
   
    virtual void Init() {
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
    };

    virtual void Update() {
        Effect::Update();
        quad.Update();
        manyQuads.Update();
    };

    virtual void Draw() {
        Programs& progs = Programs::GetInstance();
        Effect::Draw();
        surface.Bind();
        surfaceCam = context->mainCam;
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

        context->viewport.Bind();
        context->mainCam.Bind(quad.modelMat);
        manyQuads.Draw();
        pezCheckGL("Quads::Draw draw failed"); 
        glBindTexture(GL_TEXTURE_2D, 0);
    };
};

