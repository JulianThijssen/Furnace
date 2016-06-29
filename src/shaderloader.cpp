#include "shaderloader.h"

#include <fstream>
#include <QDebug>

const int ShaderLoader::LOG_SIZE = 1024;

Shader* ShaderLoader::loadShaders(std::string vertPath, std::string fragPath) {
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

GLuint ShaderLoader::loadShader(std::string path, int type) {
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    int handle = 0;

    qDebug() << "Loading shader file: " << path.c_str();
    // Read all lines and append together
    std::ifstream file(path);
    if (file.fail() || !file.is_open()) {
        qDebug() << "No such file: " << path.c_str();
    }

    std::string source;
    std::string line;
    while (std::getline(file, line)) {
        source.append(line + "\n");
    }

    const char* csource = source.c_str();

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
        qDebug() << "Successfully compiled shader: " << path.c_str();
    }

    return handle;
}
