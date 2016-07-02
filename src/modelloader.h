#ifndef MODELLOADER_H
#define MODELLOADER_H

#include <scene.h>

#include "Model.h"

class ModelLoader
{
public:
    ModelLoader();

    static Model* loadModel(const char* path, const bool resize);
    static Model* uploadModel(const aiScene& scene, const bool resize);
};

#endif // MODELLOADER_H
