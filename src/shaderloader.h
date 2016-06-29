#ifndef SHADERLOADER_H
#define SHADERLOADER_H

#include "Shader.h"

#include <QOpenGLFunctions>
#include <string>

class ShaderLoader
{
public:
    static const int LOG_SIZE;

    static Shader* loadShaders(std::string vertPath, std::string fragPath);
    static GLuint loadShader(std::string path, int type);
};

#endif // SHADERLOADER_H
