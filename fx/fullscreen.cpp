#include "glm/gtc/type_ptr.hpp"
#include "fx/fullscreen.h"
#include "common/programs.h"
#include "common/init.h"

using namespace std;
using namespace glm;

Fullscreen::Fullscreen(Mask mask, Effect* child) :
    Effect(), _mask(mask), _depthPeer(0)
{
    clearColor = glm::vec4(0.1, 0.2, 0.4, 1);
    if (child) {
        _children.push_back(child);
    }
}

Fullscreen::Fullscreen(string customProgram, Mask mask) :
    Effect(), _mask(mask), _customProgram(customProgram), _depthPeer(0)
{
    clearColor = glm::vec4(0, 0, 0, 0);
}

void
Fullscreen::Init()
{
    name = "Fullscreen";
    Effect::Init();
    Programs& progs = Programs::GetInstance();

    ivec2 size = ivec2(PezGetConfig().Width, PezGetConfig().Height);

    if (_customProgram.empty()) {
        glUseProgram(progs.Load("Fullscreen"));
        glUniform1i(u("ApplySolidColor"), _mask & SolidColorFlag);
        glUniform1i(u("ApplyVignette"),   _mask & VignetteFlag);
        glUniform1i(u("ApplyScanLines"),  _mask & ScanLinesFlag);
        glUniform1i(u("ApplyTeleLines"),  _mask & TeleLinesFlag);
        glUniform1i(u("ApplyCopyDepth"),  _mask & CopyDepthFlag);
        if (_mask & ScanLinesFlag) {
            size.y /= 2;
        }
    } else {
        progs.Load(_customProgram);
    }

    _emptyVao.InitEmpty();

    GLenum internalFormat = GL_RGBA8;
    GLenum format = GL_RGBA;
    GLenum type = GL_UNSIGNED_BYTE;
    GLenum filter = (_mask & MipmapsFlag) ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;
    GLenum createDepth = true;
    if (_depthPeer) {
        _surface.Init(size, internalFormat, format, type, filter, false, &(_depthPeer->_surface));
    } else {
        _surface.Init(size, internalFormat, format, type, filter, createDepth);
    }

    pezCheckGL("Fullscreen::Init");

    FOR_EACH(child, _children) {
        (*child)->Init();
    }
}

void
Fullscreen::Update()
{
    Effect::Update();
    FOR_EACH(child, _children) {
        (*child)->Update();
    }
}

void
Fullscreen::AddChild(Effect* child)
{
    _children.push_back(child);
}

void
Fullscreen::ShareDepth(Fullscreen* depthPeer)
{
    _depthPeer = depthPeer;
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
    if (_children.size()) {
        glClearColor(clearColor.r, clearColor.g,
                     clearColor.b, clearColor.a);
        GLbitfield clearMask = GL_COLOR_BUFFER_BIT;
        if (!_depthPeer) {
            clearMask |= GL_DEPTH_BUFFER_BIT;
        }
        glClear(clearMask);
        FOR_EACH(child, _children) {
            (*child)->Draw();
        }
    }

    glBindFramebuffer(GL_FRAMEBUFFER, previousFb);
    glViewport(previousVp[0], previousVp[1], previousVp[2], previousVp[3]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _surface.texture);

    if (_mask & MipmapsFlag) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _surface.depthTexture);
    glActiveTexture(GL_TEXTURE0);

    Programs& progs = Programs::GetInstance();

    if (_customProgram.size()) {
        glUseProgram(progs[_customProgram]);
    } else {
        glUseProgram(progs["Fullscreen"]);
    }

    glUniform4fv(u("SolidColor"), 1, ptr(solidColor));

    glUniform1i(u("SourceImage"), 0);
    glUniform1i(u("DepthImage"), 1);

    vec2 inverseViewport = 1.0f /
        vec2(previousVp[2], previousVp[3]);
    glUniform2fv(u("InverseViewport"), 1, ptr(inverseViewport));

    _emptyVao.Bind();

    if (_mask & BlendFlag) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    glDisable(GL_DEPTH_TEST);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glEnable(GL_DEPTH_TEST);

    glDisable(GL_BLEND);

    glBindTexture(GL_TEXTURE_2D, 0);
    pezCheckGL("Fullscreen::Draw");
}
