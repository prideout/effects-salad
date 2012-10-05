#include "surface.h"

using glm::ivec2;

Surface::Surface() : width(0),
                     height(0),
                     texture(0),
                     depthTexture(0),
                     fbo(0) {
}

void
Surface::Init() {
    ivec2 size = ivec2(256);
    GLenum internalFormat = GL_RGB16;
    GLenum format = GL_RGBA;
    GLenum type = GL_FLOAT;
    GLenum filter = GL_NEAREST;
    bool createDepth = true;
    this->Init(size, internalFormat, format, type, filter, createDepth);
}

void
Surface::Init(ivec2 size,
              GLenum internalFormat,
              GLenum format,
              GLenum type,
              GLenum filter,
              Mask flags,
              Surface* depthPeer)
{
    bool createDepth = (flags & DepthFlag);

    width = size.x;
    height = size.y;

    // beware of mac limitations:
    // http://developer.apple.com/library/mac/#documentation/Darwin/Reference/Manpages/man3/glTexImage2D.3.html

    bool mipmapped = (filter == GL_LINEAR_MIPMAP_LINEAR);
    GLenum minFilter = filter;
    GLenum magFilter = mipmapped ? GL_LINEAR : filter;

    // create a color texture
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // XXX: on OSX, params *must* be set first
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    pezCheckGL("Creation of the color texture for the FBO");

    // share or create depth texture
    if (depthPeer) {
        createDepth = false;
        pezCheck(depthPeer->depthTexture, "DepthPeer has no depth buffer.  Has it been initialized?");
        depthTexture = depthPeer->depthTexture;
    } else if (createDepth) {
        glGenTextures(1, &depthTexture);
        glBindTexture(GL_TEXTURE_2D, depthTexture);
        GLenum type = GL_FLOAT;
        GLenum internalFormat = GL_DEPTH_COMPONENT24;
        GLenum format = GL_DEPTH_COMPONENT;
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_2D, 0);
        pezCheckGL("Creation of the depth texture for the FBO");
    } else {
        depthTexture = 0;
    }

    // create FBO itself
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
    if (depthTexture) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
    }

    if (flags & NormalsFlag) {
        glGenTextures(1, &normalsTexture);
        glBindTexture(GL_TEXTURE_2D, normalsTexture);
        GLenum type = GL_FLOAT;
        GLenum internalFormat = GL_RGBA32F;
        GLenum format = GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_2D, 0);
        pezCheckGL("Creation of the normals texture for the FBO");

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normalsTexture, 0);
        pezCheckGL("Attachment of the normals texture for the FBO");
    }

    if (flags & PositionsFlag) {
        glGenTextures(1, &positionsTexture);
        glBindTexture(GL_TEXTURE_2D, positionsTexture);
        GLenum type = GL_FLOAT;
        GLenum internalFormat = GL_RGBA32F;
        GLenum format = GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_2D, 0);
        pezCheckGL("Creation of the normals texture for the FBO");
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, normalsTexture, 0);
        pezCheckGL("Attachment of the positions texture for the FBO");
    }

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

