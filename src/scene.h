#ifndef SCENE_H
#define SCENE_H

#include "mesh.h"
#include "model.h"
#include "image.h"

#include <QOpenGLTexture>
#include <QOpenGLFunctions>

class Scene {
public:
    Scene() :
        quad(nullptr),
        lowPoly(nullptr),
        highPoly(nullptr),
        cage(nullptr),
        texture(nullptr),
        normalMap(nullptr)
    { }

    Model* quad;
    Model* lowPoly;
    Model* highPoly;
    Mesh* cage;

    QOpenGLTexture* texture;
    GLuint debugNormals = 0;
    GLuint bakedNormal = 0;

    unsigned int resolution = 1024;
    Image* normalMap;
};

#endif // SCENE_H
