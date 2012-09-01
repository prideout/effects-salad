#include "glm/gtc/type_ptr.hpp"
#include "fx/fullscreen.h"
#include "common/programs.h"
#include "common/init.h"

using namespace std;
using namespace glm;

void
Fullscreen::Init()
{
    name = "Fullscreen";
    Effect::Init();
    Programs& progs = Programs::GetInstance();
    glUseProgram(progs.Load("Fullscreen"));

    glUniform1i(u("ApplySolidColor"), _mask & SolidColorFlag);
    glUniform1i(u("ApplyVignette"),   _mask & VignetteFlag);
    glUniform1i(u("ApplyScanLines"),  _mask & ScanLinesFlag);

    glGenVertexArrays(1, &_vao);

    ivec2 size = ivec2(PezGetConfig().Width, PezGetConfig().Height);
    GLenum internalFormat = GL_RGBA8;
    GLenum format = GL_RGBA;
    GLenum type = GL_UNSIGNED_BYTE;
    GLenum filter = GL_LINEAR;
    GLenum createDepth = true;
    _surface.Init(size, internalFormat, format, type, filter, createDepth);

    pezCheckGL("Fullscreen::Init");

    if (_child) {
        _child->Init();
    }
}

void
Fullscreen::Update()
{
    Effect::Update();
    if (_child) {
        _child->Update();
    }
}

void
Fullscreen::Draw()
{
    Effect::Draw();

    int previousVp[4];
    glGetIntegerv(GL_VIEWPORT, previousVp);
    pezCheck(previousVp[0] == 0 and previousVp[1] == 0, "Sliding viewports not yet supported");

    GLint previousFb;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &previousFb);
    glBindFramebuffer(GL_FRAMEBUFFER, _surface.fbo);
    glViewport(0, 0, _surface.width, _surface.height);
    if (_child) {
        glClearColor(0.1, 0.2, 0.4, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        _child->Draw();
    }

    glBindFramebuffer(GL_FRAMEBUFFER, previousFb);
    glViewport(previousVp[0], previousVp[1], previousVp[2], previousVp[3]);
    glBindTexture(GL_TEXTURE_2D, _surface.texture);

    Programs& progs = Programs::GetInstance();

    glUseProgram(progs["Fullscreen"]);
    glUniform4fv(u("SolidColor"), 1, ptr(solidColor));
    glUniform1i(u("SourceImage"), 0);
    vec2 inverseViewport = 1.0f /
        vec2(previousVp[2], previousVp[3]);
    glUniform2fv(u("InverseViewport"), 1, ptr(inverseViewport));

    glBindVertexArray(_vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindTexture(GL_TEXTURE_2D, 0);
    pezCheckGL("Fullscreen::Draw");
}
