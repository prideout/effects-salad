#pragma once

#include "common/effect.h"
#include "common/instancer.h"
#include "common/texture.h"
#include "common/quad.h"


// simple effect used to test framework features

class Quads : public Effect {
public:
    Quad quad;
    Instancer manyQuads;
    BufferTexture tex;

    Quads() : Effect() {}
    virtual ~Quads() {} ;
   
    virtual void Init() {
        Programs& progs = Programs::GetInstance();
        glUseProgram(progs.Load("Default.Simple"));
        glUseProgram(progs.Load("Default.Instanced", 
                                "Default.Simple.FS", 
                                "Default.Instanced.VS"));

        pezCheck(glGetError() == GL_NO_ERROR, "compile failed");

        quad.Init();
        manyQuads.vao = quad.vao;
        manyQuads.Init();

        float x[] = {1.5,-1.5,200,300,40,40,40,40,40,40,40,40,40,40,40,40};
        
        tex.Init(GL_R32F, 16*sizeof(float), x);
        pezCheck(glGetError() == GL_NO_ERROR, "Texture init failed");
        
        glUniform1i(u("Offsets"), 0);
        pezCheck(glGetError() == GL_NO_ERROR, "Texture init failed1");
        
        glActiveTexture(GL_TEXTURE0);
        pezCheck(glGetError() == GL_NO_ERROR, "Texture init failed2");
        
        tex.Bind();
        pezCheck(glGetError() == GL_NO_ERROR, "Texture bind failed3");
    };

    virtual void Update() {
        quad.Update();
        manyQuads.Update();
    };

    virtual void Render() {
        context->mainCam.Bind(quad.modelMat);
        //quad.Draw();
        manyQuads.Draw();
        pezCheck(glGetError() == GL_NO_ERROR, "draw failed"); 
    };
};

