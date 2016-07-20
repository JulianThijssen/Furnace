#include "cage.h"

#include "mesh.h"
#include "modelloader.h"
#include "util/vector3f.h"

#include <QDebug>

std::vector<std::vector<int>> Cage::vertexBuckets;

int Cage::generateHash(const Vector3f& v, int numBuckets) {
    float hash = fabs(v.x + v.y + v.z) * numBuckets;
    int bucket = (int) floor(hash);
    if (bucket > numBuckets - 1) { bucket = numBuckets - 1; }

    return bucket;
}

//Mesh* Cage::generateCage(const Mesh& mesh, const float offset) {
//    Mesh* cage = new Mesh();

//    cage->indices = mesh.indices;
//    cage->faces = mesh.faces;
//    cage->texCoords = mesh.texCoords;
//    cage->normals = mesh.normals;
//    cage->tangents = mesh.tangents;
//    cage->bitangents = mesh.bitangents;
//    cage->numFaces = mesh.numFaces;

//    qDebug() << "Generating cage...";

//    // Initialise buckets
//    int numBuckets = 1000;
//    vertexBuckets.resize(numBuckets, std::vector<int>());

//    // Divide mesh vertex indices into their allotted buckets
//    for (int i = 0; i < mesh.vertices.size(); i++) {
//        Vector3f rv = mesh.vertices[i];

//        int bucket = generateHash(rv, numBuckets);

//        vertexBuckets[bucket].push_back(i);
//    }

//    // Find similar vertices and calculate average normal
//    for (int i = 0; i < mesh.vertices.size(); i++) {
//        Vector3f rv = mesh.vertices[i];

//        Vector3f rayDir(0, 0, 0);

//        int bucket = generateHash(rv, numBuckets);

//        for (int b = -1; b <= 1; b++) {
//            // Also look in neighbouring buckets
//            int tbucket = bucket + b;
//            if (tbucket < 0) { tbucket = 0; }
//            if (tbucket > numBuckets - 1) { tbucket = numBuckets - 1; }

//            // Get list of vertices in current bucket
//            std::vector<int> vBucket = vertexBuckets[tbucket];

//            // For every
//            for (int j = 0; j < vBucket.size(); j++) {
//                int bucketIndex = vBucket[j];
//                Vector3f v = mesh.vertices[bucketIndex];
//                if ((rv - v).length() < 0.001f) {
//                    rayDir += mesh.normals[bucketIndex];
//                }
//            }
//        }

//        rayDir.normalise();

//        Vector3f cageVertex = rv + rayDir * offset;

//        cage->vertices.push_back(cageVertex);
//        cage->raydirs.push_back(-rayDir);
//    }

//    //for (unsigned int i = 0; i < numBuckets; i++) {
//    //	std::cout << "bucket #" << i << " contains:";
//    //		std::cout << " " << vertexBuckets[i].size();
//    //	std::cout << std::endl;
//    //}
//    qDebug() << "Precage vertices: " << mesh.vertices.size();
//    qDebug() << "Cage vertices: " << cage->vertices.size();

//    //qDebug() << "Generating cage took: " << timer.getTime() << " seconds";

//    ModelLoader::uploadMesh(*cage);

//    return cage;
//}

Mesh* Cage::generateCage(const Mesh& mesh, const float offset) {
    Mesh* cage = new Mesh();

    cage->indices = mesh.indices;
    cage->texCoords = mesh.texCoords;
    cage->normals = mesh.normals;
    cage->tangents = mesh.tangents;
    cage->bitangents = mesh.bitangents;
    cage->numFaces = mesh.numFaces;

    // Find similar vertices and calculate average normal
    for (int i = 0; i < mesh.vertices.size(); i++) {
        Vector3f rv = mesh.vertices[i];
        Vector3f rn = mesh.normals[i];
        Vector3f rayDir(0, 0, 0);
        rayDir.set(rn);

        rayDir.normalise();

        Vector3f cageVertex = rv + rayDir * offset;

        cage->vertices.push_back(cageVertex);
        cage->raydirs.push_back(-rayDir);
    }

    qDebug() << "Precage vertices: " << mesh.vertices.size();
    qDebug() << "Cage vertices: " << cage->vertices.size();

    //qDebug() << "Generating cage took: " << timer.getTime() << " seconds";

    ModelLoader::uploadMesh(*cage);

    return cage;
}

