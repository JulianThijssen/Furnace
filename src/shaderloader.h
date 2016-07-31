#ifndef SHADERLOADER_H
#define SHADERLOADER_H

#include "Shader.h"

#include <QOpenGLFunctions>
#include <QString>

class ShaderLoader
{
public:
    static const int LOG_SIZE;

    static Shader* loadShaders(QString vertPath, QString fragPath);
    static GLuint loadShader(QString path, int type);
};

#endif // SHADERLOADER_H
