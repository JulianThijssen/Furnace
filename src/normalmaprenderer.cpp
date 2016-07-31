#include "normalmaprenderer.h"

#include "shaderloader.h"
#include "modelloader.h"
#include "textureloader.h"
#include "util/math.h"

#include <vector>
#include <ctime>

#include <QOpenGLTimerQuery>
#include <QtMath>
#include <QDebug>

const int TILES = 8;

NormalMapRenderer::NormalMapRenderer() :
    diffuseShader(nullptr),
    normalShader(nullptr),
    tilesShader(nullptr)
{

}

NormalMapRenderer::~NormalMapRenderer()
{
    delete diffuseShader;
    delete normalShader;
    delete tilesShader;
    qDebug() << "NormalMapRenderer destroyed!";
}

void NormalMapRenderer::save(QString fileName, Image* normalMap) {
    TextureLoader::storeTexture(fileName.toStdString().c_str(), normalMap);
}

void NormalMapRenderer::init() {
    initializeOpenGLFunctions();

    bakeBuffer = new Framebuffer();

    diffuseShader = ShaderLoader::loadShaders(":/res/diffuse.vert", ":/res/diffuse.frag");
    normalShader = ShaderLoader::loadShaders(":/res/normal.vert", ":/res/normal.frag");
    tilesShader = ShaderLoader::loadShaders(":/res/tiles.vert", ":/res/tiles.frag");

    glClearColor(0, 0, 0, 1);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

void NormalMapRenderer::resize(int w, int h) {
    Matrix4f projMatrix;
    float fovyr = qDegreesToRadians(90.f);
    projMatrix[0] = (float)(1 / tan(fovyr / 2)) / ((float) w / h);
    projMatrix[5] = (float)(1 / tan(fovyr / 2));
    projMatrix[10] = (0.1f + 100) / (0.1f - 100);
    projMatrix[11] = -1;
    projMatrix[14] = (2 * 0.1f * 100) / (0.1f - 100);
    projMatrix[15] = -0;

    diffuseShader->bind();
    diffuseShader->uniformMatrix4f("projMatrix", projMatrix);
    diffuseShader->unbind();

    projMatrix.setIdentity();
    projMatrix[0] = 2 / (1 - 0);
    projMatrix[5] = 2 / (1 - 0);
    projMatrix[10] = -2 / (1 - -1);
    projMatrix[12] = (-1 - 0) / (1 - 0);
    projMatrix[13] = (-1 - 0) / (1 - 0);
    projMatrix[14] = (-1 - -1) / (1 - -1);

    normalShader->bind();
    normalShader->uniformMatrix4f("projMatrix", projMatrix);
    normalShader->unbind();
}

void NormalMapRenderer::render(const Scene& scene) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (scene.bakedNormal) {
        glClear(GL_DEPTH_BUFFER_BIT);

        diffuseShader->bind();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, scene.bakedNormal);
        diffuseShader->uniform1i("albedo", 0);
        diffuseShader->uniform1i("hasAlbedo", 1);

        Matrix4f modelMatrix;
        modelMatrix.setIdentity();
        modelMatrix.translate(Vector3f(0, 0, -1.f));
        diffuseShader->uniformMatrix4f("modelMatrix", modelMatrix);

        Mesh mesh = scene.quad->meshes[0];
        glBindVertexArray(mesh.handle);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.faceVBO);
        glDrawElements(GL_TRIANGLES, (GLsizei) mesh.indices.size(), GL_UNSIGNED_INT, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        diffuseShader->unbind();
    }
}

void NormalMapRenderer::renderNormal(Scene& scene) {
    std::clock_t begin = clock();

//    makeCurrent(); FIXME can this go?

qDebug() << "Rendering normal..";
    const int width = scene.resolution;
    const int height = width;qDebug() << "Rendering normal..1";
    bakeBuffer->setTexture(width, height);qDebug() << "Rendering normal..1.5";
    bakeBuffer->enable();
qDebug() << "Rendering normal..2";
    glClearColor(0.5f, 0.5f, 1, 1);qDebug() << "Rendering normal..3";
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
qDebug() << "Rendering normal..4";
    Mesh mesh = scene.highPoly->meshes[0];
    qDebug() << "Sizes: " << mesh.indices.size() << ", " << mesh.vertices.size() << ", " << mesh.normals.size();

    const uint aWidth = Math::ceilToPow2(Math::sqrt((float) mesh.indices.size()));
    const uint aHeight = aWidth;
    qDebug() << "Width: " << aWidth << " Height: " << aHeight;

    std::vector<float> triList;
    triList.resize(aWidth * aHeight, 0);
    for (uint i = 0; i < mesh.indices.size(); i++) {
        triList[i] = (float) mesh.indices[i];
    }

    std::vector<float> vertexList;
    vertexList.resize(aWidth * aHeight * 4, 0);
    for (uint i = 0; i < mesh.vertices.size(); i++) {
        vertexList[i * 4 + 0] = mesh.vertices[i].x;
        vertexList[i * 4 + 1] = mesh.vertices[i].y;
        vertexList[i * 4 + 2] = mesh.vertices[i].z;
        vertexList[i * 4 + 3] = 1;
    }

    std::vector<float> normalList;
    normalList.resize(aWidth * aHeight * 4, 0);
    for (uint i = 0; i < mesh.normals.size(); i++) {
        normalList[i * 4 + 0] = mesh.normals[i].x;
        normalList[i * 4 + 1] = mesh.normals[i].y;
        normalList[i * 4 + 2] = mesh.normals[i].z;
        normalList[i * 4 + 3] = 0;
    }

    GLuint triListTex = TextureLoader::createArray(aWidth, aHeight, GL_R32F, GL_RED, GL_FLOAT, &triList[0]);
    GLuint vertexTex = TextureLoader::createArray(aWidth, aHeight, GL_RGBA32F, GL_RGBA, GL_FLOAT, &vertexList[0]);
    GLuint normalTex = TextureLoader::createArray(aWidth, aHeight, GL_RGBA32F, GL_RGBA, GL_FLOAT, &normalList[0]);

    float tileSize = 1.0f / TILES;

    normalShader->bind();

    // Upload triangle lists
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, triListTex);
    normalShader->uniform1i("triList", 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, vertexTex);
    normalShader->uniform1i("vertexList", 1);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, normalTex);
    normalShader->uniform1i("normalList", 2);

    Matrix4f projMatrix;

    scene.cage->bind();

    for (unsigned int x = 0; x < TILES; x++) {
        for (unsigned int y = 0; y < TILES; y++) {
            //qDebug() << x << " " << y;
            qDebug() << "X: " << -1+x*tileSize << " Y: " << -1+y*tileSize;

            float left = 0 + x * tileSize;
            float right = 0 + (x+1) * tileSize;
            float bottom = 0 + y * tileSize;
            float top = 0 + (y+1) * tileSize;
            float zNear = -1;
            float zFar = 1;

            projMatrix.setIdentity();
            projMatrix[0] = 2 / (right - left);
            projMatrix[5] = 2 / (top - bottom);
            projMatrix[10] = -2 / (zFar - zNear);
            projMatrix[12] = (-right - left) / (right - left);
            projMatrix[13] = (-top - bottom) / (top - bottom);
            projMatrix[14] = (-zFar - zNear) / (zFar - zNear);
            float tileWidth = width / TILES;
            float tileHeight = height / TILES;
            glViewport(x * tileWidth, y * tileHeight, tileWidth, tileHeight);

            normalShader->uniformMatrix4f("projMatrix", projMatrix);

            normalShader->uniform1i("arrayWidth", aWidth);
            normalShader->uniform1i("arrayHeight", aHeight);

            glDrawElements(GL_TRIANGLES, (GLsizei) scene.cage->indices.size(), GL_UNSIGNED_INT, 0);

            glFinish();
        }
    }

    scene.cage->unbind();

    unsigned char *pixels = new unsigned char[width * height * 4];
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    scene.normalMap = new Image(width, height);
    scene.normalMap->setData(pixels);

    scene.bakedNormal = bakeBuffer->getTexture();
    bakeBuffer->disable();

    std::clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    qDebug() << "Rendering took: " << elapsed_secs;
    //qDebug() << "Rendering took" << timer.waitForResult() << "microseconds";
}
