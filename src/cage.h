#ifndef CAGE_H
#define CAGE_H

#include <unordered_map>

class Mesh;
class Vector3f;

class Cage {
public:
    static Mesh* generateCage(const Mesh& mesh, const float offset);

    static std::vector<std::vector<int>> vertexBuckets;
private:
    static int generateHash(const Vector3f& v, int numBuckets);
};

#endif // CAGE_H
