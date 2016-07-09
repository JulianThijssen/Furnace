#ifndef MESH_H
#define MESH_H

#include "Face.h"

#include "util/vector3f.h"
#include "util/vector2f.h"

#include <vector>
#include <string>

class Mesh {
public:
    std::string name;
    std::vector<Vector3f> vertices;
    std::vector<Vector2f> texCoords;
    std::vector<Vector3f> normals;
    std::vector<Vector3f> tangents;
    std::vector<Vector3f> bitangents;
    std::vector<Vector3f> raydirs;
    std::vector<Face> faces;

    unsigned int handle;
    unsigned int numFaces;
    std::vector<unsigned int> indices;
    unsigned int faceVBO;
};

#endif // MESH_H
