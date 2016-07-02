#ifndef SHADER_H
#define SHADER_H

#include <QOpenGLFunctions_3_2_Core>

#include <unordered_map>
#include <string>

class Matrix4f;

class Shader {
public:
    GLuint handle;

    Shader(GLuint handle);
    void bind();
    void unbind();
    int location(const char* uniform);
    void uniform1i(const char* name, int value);
    void uniform1f(const char* name, float value);
    void uniform3f(const char* name, float v0, float v1, float v2);
    void uniformMatrix4f(const char* name, Matrix4f& m);
private:
    std::unordered_map<std::string, int> locationMap;
};

#endif // SHADER_H
