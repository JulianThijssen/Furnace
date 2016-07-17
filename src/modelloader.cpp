#include "modelloader.h"

#include <Importer.hpp>
#include <postprocess.h>

#include "Model.h"
#include "Mesh.h"

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

GLuint ModelLoader::createDebugNormals(Mesh& mesh, const float length) {
    QOpenGLFunctions_3_2_Core *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_2_Core>();

    std::vector<Vector3f> lines;
    lines.resize(mesh.vertices.size() * 2);
    for (int i = 0; i < mesh.vertices.size(); i++) {
        Vector3f v(mesh.vertices[i]);
        lines[i*2+0] = v;
        lines[i*2+1] = v + Vector3f(mesh.normals[i]) * 0.1f;
    }

    GLuint normalVAO;
    f->glGenVertexArrays(1, &normalVAO);
    f->glBindVertexArray(normalVAO);
    GLuint buffer;
    f->glGenBuffers(1, &buffer);
    f->glBindBuffer(GL_ARRAY_BUFFER, buffer);
    f->glBufferData(GL_ARRAY_BUFFER, lines.size() * 3 * sizeof(float), &lines[0], GL_STATIC_DRAW);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
    f->glEnableVertexAttribArray(0);
    f->glBindVertexArray(0);

    return normalVAO;
}

void ModelLoader::uploadMesh(Mesh& mesh) {
    QOpenGLFunctions_3_2_Core *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_2_Core>();

    // Generate vertex array object
    GLuint vao;
    f->glGenVertexArrays(1, &vao);
    f->glBindVertexArray(vao);

    // Store faces in a buffer
    GLuint faceVBO;
    f->glGenBuffers(1, &faceVBO);
    f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceVBO);
    f->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh.numFaces * 3, &mesh.indices[0], GL_STATIC_DRAW);
    f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Store vertices in a buffer
    if (mesh.vertices.size()) {
        GLuint vertexVBO;
        f->glGenBuffers(1, &vertexVBO);
        f->glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
        f->glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh.vertices.size() * 3, &mesh.vertices[0], GL_STATIC_DRAW);
        f->glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);
        f->glEnableVertexAttribArray(0);
    }

    // Store texture coordinates in a buffer
    if (mesh.texCoords.size()) {
        GLuint texCoordVBO;
        f->glGenBuffers(1, &texCoordVBO);
        f->glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
        f->glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh.texCoords.size() * 2, &mesh.texCoords[0], GL_STATIC_DRAW);
        f->glVertexAttribPointer(1, 2, GL_FLOAT, 0, 0, 0);
        f->glEnableVertexAttribArray(1);
    }

    // Store normals in a buffer
    if (mesh.normals.size()) {
        GLuint normalVBO;
        f->glGenBuffers(1, &normalVBO);
        f->glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
        f->glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh.normals.size() * 3, &mesh.normals[0], GL_STATIC_DRAW);
        f->glVertexAttribPointer(2, 3, GL_FLOAT, 0, 0, 0);
        f->glEnableVertexAttribArray(2);
    }

    // Store tangents in a buffer
    if (mesh.tangents.size()) {
        GLuint tangentVBO;
        f->glGenBuffers(1, &tangentVBO);
        f->glBindBuffer(GL_ARRAY_BUFFER, tangentVBO);
        f->glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh.tangents.size() * 3, &mesh.tangents[0], GL_STATIC_DRAW);
        f->glVertexAttribPointer(3, 3, GL_FLOAT, 0, 0, 0);
        f->glEnableVertexAttribArray(3);

        GLuint bitangentVBO;
        f->glGenBuffers(1, &bitangentVBO);
        f->glBindBuffer(GL_ARRAY_BUFFER, bitangentVBO);
        f->glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh.bitangents.size() * 3, &mesh.bitangents[0], GL_STATIC_DRAW);
        f->glVertexAttribPointer(4, 3, GL_FLOAT, 0, 0, 0);
        f->glEnableVertexAttribArray(4);
    }

    // Store ray directions
    if (mesh.raydirs.size()) {
        GLuint raydirVBO;
        f->glGenBuffers(1, &raydirVBO);
        f->glBindBuffer(GL_ARRAY_BUFFER, raydirVBO);
        f->glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh.raydirs.size() * 3, &mesh.raydirs[0], GL_STATIC_DRAW);
        f->glVertexAttribPointer(5, 3, GL_FLOAT, 0, 0, 0);
        f->glEnableVertexAttribArray(5);
    }

    // Unbind the buffers
    f->glBindBuffer(GL_ARRAY_BUFFER, 0);
    f->glBindVertexArray(0);

    // Store relevant data in the new mesh
    mesh.handle = vao;
    mesh.faceVBO = faceVBO;
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

        qDebug() << "Model vertices: " << aiMesh->mNumVertices;
        qDebug() << "Model faces: " << aiMesh->mNumFaces;

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
            f->glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
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
            f->glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, 0);
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
            f->glVertexAttribPointer(2, 3, GL_FLOAT, false, 0, 0);
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
