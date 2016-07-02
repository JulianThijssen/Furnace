#include "shader.h"

#include "Matrix4f.h"
#include <QDebug>

Shader::Shader(GLuint handle) {
    this->handle = handle;
}

void Shader::bind() {
    QOpenGLFunctions_3_2_Core *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_2_Core>();
    f->glUseProgram(handle);
}

void Shader::unbind() {
    QOpenGLFunctions_3_2_Core *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_2_Core>();
    f->glUseProgram(0);
}

void Shader::uniform1i(const char* name, int value) {
    QOpenGLFunctions_3_2_Core *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_2_Core>();
    f->glUniform1i(location(name), value);
}

void Shader::uniform1f(const char* name, float value) {
    QOpenGLFunctions_3_2_Core *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_2_Core>();
    f->glUniform1f(location(name), value);
}

void Shader::uniform3f(const char* name, float v0, float v1, float v2) {
    QOpenGLFunctions_3_2_Core *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_2_Core>();
    f->glUniform3f(location(name), v0, v1, v2);
}

void Shader::uniformMatrix4f(const char* name, Matrix4f& m) {
    QOpenGLFunctions_3_2_Core *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_2_Core>();
    f->glUniformMatrix4fv(location(name), 1, false, m.toArray());
}

int Shader::location(const char* name) {
    QOpenGLFunctions_3_2_Core *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_2_Core>();

    std::unordered_map<std::string, int>::const_iterator it = locationMap.find(std::string(name));
    if (it != locationMap.end()) {
        return it->second;
    } else {
        int location = f->glGetUniformLocation(handle, name);
        locationMap[std::string(name)] = location;
        return location;
    }
}
