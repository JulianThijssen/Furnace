#ifndef MODEL_H
#define MODEL_H

#include "Mesh.h"

#include <vector>

class Model {
public:
    std::vector<Mesh> meshes;

    void addMesh(const Mesh& mesh) {
        meshes.push_back(mesh);
    }
};

#endif // MODEL_H
