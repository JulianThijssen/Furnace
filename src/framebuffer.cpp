#include "framebuffer.h"

#include <cstdio>

#include <QOpenGLFunctions_3_2_Core>

#include <QDebug>

Framebuffer::Framebuffer() {
    QOpenGLFunctions_3_2_Core *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_2_Core>();
    f->glGenFramebuffers(1, &buffer);
}

GLuint Framebuffer::getTexture() {
    return colorTex;
}

void Framebuffer::setTexture(GLsizei width, GLsizei height) {
    this->width = width;
    this->height = height;

    QOpenGLFunctions_3_2_Core *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_2_Core>();

    f->glGenTextures(1, &colorTex);

    f->glBindFramebuffer(GL_FRAMEBUFFER, buffer);
    f->glBindTexture(GL_TEXTURE_2D, colorTex);
    f->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    f->glBindTexture(GL_TEXTURE_2D, 0);

    f->glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, colorTex, 0);
    GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
    f->glDrawBuffers(1, drawBuffers);

    f->glGenRenderbuffers(1, &stencil);
    f->glBindRenderbuffer(GL_RENDERBUFFER, stencil);
    f->glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    f->glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, stencil);
    f->glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, stencil);

    if (f->glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        printf("The framebuffer is not happy\n");
    }
}

void Framebuffer::enable() {
    QOpenGLFunctions_3_2_Core *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_2_Core>();

    f->glBindFramebuffer(GL_FRAMEBUFFER, buffer);
    f->glViewport(0, 0, width, height);
}

void Framebuffer::disable() {
    QOpenGLFunctions_3_2_Core *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_2_Core>();

    f->glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
