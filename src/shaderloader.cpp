#include "shaderloader.h"

#include <string>

#include <QTextStream>
#include <QFile>
#include <QDebug>

const int ShaderLoader::LOG_SIZE = 1024;

Shader* ShaderLoader::loadShaders(QString vertPath, QString fragPath) {
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    int vertexShader = loadShader(vertPath, GL_VERTEX_SHADER);
    int fragmentShader = loadShader(fragPath, GL_FRAGMENT_SHADER);

    int shaderProgram = f->glCreateProgram();

    f->glAttachShader(shaderProgram, vertexShader);
    f->glAttachShader(shaderProgram, fragmentShader);

    f->glLinkProgram(shaderProgram);
    f->glValidateProgram(shaderProgram);

    int handle = shaderProgram;

    Shader* shader = new Shader(handle);

    return shader;
}

GLuint ShaderLoader::loadShader(QString path, int type) {
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    int handle = 0;

    qDebug() << "Loading shader file: " << path;
    // Read all lines and append together
    QFile file(path);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "No such file: " << path;
        return 0;
    }
    QTextStream in(&file);

    QString source;
    source.append(in.readAll());

    std::string ssource = source.toStdString();
    const char* csource = ssource.c_str();

    // Create the shader
    handle = f->glCreateShader(type);
    f->glShaderSource(handle, 1, &csource, NULL);
    f->glCompileShader(handle);

    // Error checking
    char log[LOG_SIZE];
    GLint status;
    f->glGetShaderiv(handle, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE) {
        f->glGetShaderInfoLog(handle, LOG_SIZE, nullptr, log);
        qDebug() << log;
    } else {
        qDebug() << "Successfully compiled shader: " << path;
    }

    return handle;
}
