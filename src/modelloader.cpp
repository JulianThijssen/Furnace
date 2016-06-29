#include "modelloader.h"

#include <Importer.hpp>
#include <postprocess.h>

#include "Model.h"
#include "Mesh.h"

#include <QOpenGLFunctions_3_2_Core>
#include <QDebug>

ModelLoader::ModelLoader()
{

}

Model* ModelLoader::loadModel(const char* path, const bool resize) {
    Assimp::Importer importer;

    unsigned int flags = aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_SortByPType | aiProcess_GenUVCoords | aiProcess_JoinIdenticalVertices;
    const aiScene* scene = importer.ReadFile(path, flags);

    if(!scene) {
        qDebug() << importer.GetErrorString();
    }

    Model* model = uploadModel(*scene, resize);

    return model;
}

Model* ModelLoader::uploadModel(const aiScene& scene, const bool resize) {
    QOpenGLFunctions_3_2_Core *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_2_Core>();

    Model* model = new Model();

    for(unsigned int i = 0; i < scene.mNumMeshes; i++) {
        aiMesh* aiMesh = scene.mMeshes[i];
        Mesh mesh;

        mesh.indices.resize(aiMesh->mNumFaces * 3);
        for (unsigned int j = 0; j < aiMesh->mNumFaces; j++) {
            const aiFace face = aiMesh->mFaces[j];
            mesh.indices[j * 3 + 0] = face.mIndices[0];
            mesh.indices[j * 3 + 1] = face.mIndices[1];
            mesh.indices[j * 3 + 2] = face.mIndices[2];
        }

        // Generate vertex array object
        GLuint vao;
        f->glGenVertexArrays(1, &vao);
        f->glBindVertexArray(vao);

        // Store faces in a buffer
        GLuint faceVBO;
        f->glGenBuffers(1, &faceVBO);
        f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceVBO);
        f->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * aiMesh->mNumFaces * 3, &mesh.indices[0], GL_STATIC_DRAW);
        f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        // Store vertices in a buffer
        if (aiMesh->HasPositions()) {
            if (resize) {
                float size = 0;
                for (unsigned int j = 0; j < aiMesh->mNumVertices; j++) {
                    Vector3f v(aiMesh->mVertices[j].x, aiMesh->mVertices[j].y, aiMesh->mVertices[j].z);
                    if (fabs(v.x) > size) { size = fabs(v.x); }
                    if (fabs(v.y) > size) { size = fabs(v.y); }
                    if (fabs(v.z) > size) { size = fabs(v.z); }
                }

                mesh.vertices.resize(aiMesh->mNumVertices);
                for (unsigned int j = 0; j < aiMesh->mNumVertices; j++) {
                    Vector3f v(aiMesh->mVertices[j].x / size, aiMesh->mVertices[j].y / size, aiMesh->mVertices[j].z / size);
                    mesh.vertices[j] = v;
                }
            } else {
                mesh.vertices.resize(aiMesh->mNumVertices);
                for (unsigned int j = 0; j < aiMesh->mNumVertices; j++) {
                    Vector3f v(aiMesh->mVertices[j].x, aiMesh->mVertices[j].y, aiMesh->mVertices[j].z);
                    mesh.vertices[j] = v;
                }
            }

            GLuint vertexVBO;
            f->glGenBuffers(1, &vertexVBO);
            f->glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
            f->glBufferData(GL_ARRAY_BUFFER, sizeof(float) * aiMesh->mNumVertices * 3, &mesh.vertices[0], GL_STATIC_DRAW);
            f->glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);
            f->glEnableVertexAttribArray(0);
        }

        // Store texture coordinates in a buffer
        if (aiMesh->HasTextureCoords(0)) {
            mesh.texCoords.resize(aiMesh->mNumVertices);
            for (unsigned int j = 0; j < aiMesh->mNumVertices; j++) {
                Vector2f t(aiMesh->mTextureCoords[0][j].x, aiMesh->mTextureCoords[0][j].y);
                mesh.texCoords[j] = t;
            }

            GLuint texCoordVBO;
            f->glGenBuffers(1, &texCoordVBO);
            f->glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
            f->glBufferData(GL_ARRAY_BUFFER, sizeof(float) * aiMesh->mNumVertices * 2, &mesh.texCoords[0], GL_STATIC_DRAW);
            f->glVertexAttribPointer(1, 2, GL_FLOAT, 0, 0, 0);
            f->glEnableVertexAttribArray(1);
        }

        // Store normals in a buffer
        if (aiMesh->HasNormals()) {
            mesh.normals.resize(aiMesh->mNumVertices);
            for (unsigned int j = 0; j < aiMesh->mNumVertices; j++) {
                Vector3f n(aiMesh->mNormals[j].x, aiMesh->mNormals[j].y, aiMesh->mNormals[j].z);
                mesh.normals[j] = n;
            }

            GLuint normalVBO;
            f->glGenBuffers(1, &normalVBO);
            f->glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
            f->glBufferData(GL_ARRAY_BUFFER, sizeof(float) * aiMesh->mNumVertices * 3, &mesh.normals[0], GL_STATIC_DRAW);
            f->glVertexAttribPointer(2, 3, GL_FLOAT, 0, 0, 0);
            f->glEnableVertexAttribArray(2);
        }

        // Store tangents in a buffer
        if (aiMesh->HasTangentsAndBitangents()) {
            mesh.tangents.resize(aiMesh->mNumVertices);
            for (unsigned int j = 0; j < aiMesh->mNumVertices; j++) {
                Vector3f v(aiMesh->mTangents[j].x, aiMesh->mTangents[j].y, aiMesh->mTangents[j].z);
                mesh.tangents[j] = v;
            }

            mesh.bitangents.resize(aiMesh->mNumVertices);
            for (unsigned int j = 0; j < aiMesh->mNumVertices; j++) {
                Vector3f v(aiMesh->mBitangents[j].x, aiMesh->mBitangents[j].y, aiMesh->mBitangents[j].z);
                mesh.bitangents[j] = v;
            }

            GLuint tangentVBO;
            f->glGenBuffers(1, &tangentVBO);
            f->glBindBuffer(GL_ARRAY_BUFFER, tangentVBO);
            f->glBufferData(GL_ARRAY_BUFFER, sizeof(float) * aiMesh->mNumVertices * 3, aiMesh->mTangents, GL_STATIC_DRAW);
            f->glVertexAttribPointer(3, 3, GL_FLOAT, 0, 0, 0);
            f->glEnableVertexAttribArray(3);

            GLuint bitangentVBO;
            f->glGenBuffers(1, &bitangentVBO);
            f->glBindBuffer(GL_ARRAY_BUFFER, bitangentVBO);
            f->glBufferData(GL_ARRAY_BUFFER, sizeof(float) * aiMesh->mNumVertices * 3, aiMesh->mBitangents, GL_STATIC_DRAW);
            f->glVertexAttribPointer(4, 3, GL_FLOAT, 0, 0, 0);
            f->glEnableVertexAttribArray(4);
        }

        // Unbind the buffers
        f->glBindBuffer(GL_ARRAY_BUFFER, 0);
        f->glBindVertexArray(0);

        // Store relevant data in the new mesh
        mesh.handle = vao;
        mesh.numFaces = aiMesh->mNumFaces;
        mesh.faceVBO = faceVBO;

        model->addMesh(mesh);
    }

    return model;
}
