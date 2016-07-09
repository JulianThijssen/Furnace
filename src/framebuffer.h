#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <QOpenGLFunctions>

class Framebuffer {
public:
    Framebuffer();
    GLuint getTexture();
    void setTexture(GLsizei width, GLsizei height);
    void enable();
    void disable();
private:
    GLuint buffer;
    GLuint stencil;
    GLuint colorTex;
    GLsizei width;
    GLsizei height;
};

#endif // FRAMEBUFFER_H
