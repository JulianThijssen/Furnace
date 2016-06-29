#ifndef SHADER_H
#define SHADER_H

#include <QOpenGLFunctions>

class Shader {
public:
    Shader(GLuint handle) {
        this->handle = handle;
    }

    int getLocation(const char* uniform) {
        QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

        return f->glGetUniformLocation(handle, uniform);
    }

    GLuint handle;
};

#endif // SHADER_H
