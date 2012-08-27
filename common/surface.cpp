#include "surface.h"


Surface::Surface() {
}

void
Surface::Init() {

    // TODO: We should parameterize this, just doing this quick and dirty for now to get it up and running
    //       Also, should we just take a Texture object?

    width = 256;
    height = 256;
    GLenum internalFormat = GL_RGB16;
    GLenum type = GL_FLOAT;
    GLenum filter = GL_NEAREST;
    bool createDepth = true;

    // beware of mac limitations:
    // http://developer.apple.com/library/mac/#documentation/Darwin/Reference/Manpages/man3/glTexImage2D.3.html

    // create a color texture
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // XXX: on OSX, params *must* be set first
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, GL_RGBA, type, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    pezCheckGL("Creation of the color texture for the FBO");

    // create depth renderbuffer
    if (createDepth) {
        glGenRenderbuffers(1, &depth);
        glBindRenderbuffer(GL_RENDERBUFFER, depth);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
        pezCheckGL("Creation of the depth renderbuffer for the FBO");
    } else {
        depth = 0;
    }

    // create FBO itself
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
    if (depth)
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth);

    // Should we always unilaterally call Verify, or leave it up to the client?
    Verify();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    pezCheckGL("Creation of the FBO itself");
}

void
Surface::Bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, width, height);
    pezCheckGL("Frambuffer bind failed");
}

void
Surface::Unbind() {
    glViewport(0, 0, PezGetConfig().Width, PezGetConfig().Height);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void
Surface::Verify() {
    pezCheckFBO();
}

