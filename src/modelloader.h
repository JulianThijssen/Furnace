#ifndef MODELLOADER_H
#define MODELLOADER_H

#include <scene.h>

#include "model.h"
#include "mesh.h"

#include <QOpenGLFunctions_3_2_Core>
#include <QString>

class ModelLoader
{
public:
    ModelLoader();

    static Model* loadModel(QString path, const bool resize);
    static Model* loadQuad();

    static GLuint createDebugNormals(Mesh& mesh, const float length);
    static void uploadMesh(Mesh& mesh);
    static Model* uploadModel(const aiScene& scene, const bool resize);
};

#endif // MODELLOADER_H
