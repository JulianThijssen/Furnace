#include "viewwidget.h"

#include "shaderloader.h"
#include "modelloader.h"
#include "textureloader.h"
#include "model.h"
#include "mesh.h"
#include "cage.h"
#include "image.h"
#include "util/math.h"
#include <vector>
#include <ctime>

#include <QOpenGLTimerQuery>
#include <QtMath>
#include <QDebug>

const int UPDATE_RATE = 10;
const int TILES = 16;

ViewWidget::ViewWidget(QWidget* parent) :
    QOpenGLWidget(parent),
    lowPoly(nullptr),
    highPoly(nullptr),
    cage(nullptr)
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(tick()));
    timer->start(UPDATE_RATE);
}

ViewWidget::~ViewWidget() {
//    timer->stop();
    qDebug() << "Freeing resources";
    glDeleteShader(diffuseShader->handle);
    glDeleteShader(normalShader->handle);
    free(timer);
    free(diffuseShader);
    free(normalShader);
    free(lowPoly);
    free(highPoly);
    free(texture);
    qDebug() << "Ready to close...";
}

void ViewWidget::setLowPoly(QString fileName) {
    makeCurrent();
    lowPoly = ModelLoader::loadModel(fileName.toStdString().c_str(), true);

    cage = Cage::generateCage(lowPoly->meshes[0], 0.5f);
    quad = ModelLoader::loadModel("res/Quad.obj", false);
}

void ViewWidget::setHighPoly(QString fileName) {
    makeCurrent();
    highPoly = ModelLoader::loadModel(fileName.toStdString().c_str(), true);
}


void ViewWidget::tick() {
    update();
}

void ViewWidget::initializeGL() {
    initializeOpenGLFunctions();

    bakeBuffer = new Framebuffer();

    diffuseShader = ShaderLoader::loadShaders("res/diffuse.vert", "res/diffuse.frag");
    normalShader = ShaderLoader::loadShaders("res/normal.vert", "res/normal.frag");
    tilesShader = ShaderLoader::loadShaders("res/tiles.vert", "res/tiles.frag");

    texture = TextureLoader::loadTextureTrue("res/Tree.png");
    texture->create();
    qDebug() << "TEX ID: " << texture->textureId();

    glClearColor(0, 0, 0, 1);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    qDebug() << lowPoly;
}

void ViewWidget::resizeGL(int w, int h) {
    qDebug() << "RESIZING";

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

float t = 0;
void ViewWidget::paintGL() {
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (lowPoly) {
        t += 0.5f;
        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        diffuseShader->bind();

        if (bakedNormal) {
            glBindTexture(GL_TEXTURE_2D, bakedNormal);
            diffuseShader->uniform1i("albedo", 0);
            diffuseShader->uniform1i("hasAlbedo", 1);
        } else {
            diffuseShader->uniform1i("hasAlbedo", 0);
        }

        Matrix4f modelMatrix;
        modelMatrix.setIdentity();
        modelMatrix.translate(Vector3f(0, 0, -2.f));
        modelMatrix.rotate(t, 0, 1, 0);
        //modelMatrix.scale(Vector3f(0.5f, 0.5f, 0.5f));
        diffuseShader->uniformMatrix4f("modelMatrix", modelMatrix);

        Mesh mesh = lowPoly->meshes[0];
        glBindVertexArray(mesh.handle);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.faceVBO);
        glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
}

void ViewWidget::renderNormal(const int width, const int height) {
    std::clock_t begin = clock();

    makeCurrent();

qDebug() << "Rendering normal..";
    bakeBuffer->setTexture(width, height);
    bakeBuffer->enable();

    glClearColor(0.5f, 0.5f, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
qDebug() << "POST ENABLE";
    glEnable(GL_STENCIL_TEST);

    Mesh mesh = highPoly->meshes[0];
    qDebug() << "Sizes: " << mesh.indices.size() << ", " << mesh.vertices.size() << ", " << mesh.normals.size();

    const uint aWidth = Math::ceilToPow2(Math::sqrt((float) mesh.indices.size()));
    const uint aHeight = aWidth;
    qDebug() << "Width: " << aWidth << " Height: " << aHeight;

    std::vector<float> triList;
    triList.resize(aWidth * aHeight, 0);
    for (uint i = 0; i < mesh.indices.size(); i++) {
        triList[i] = (float) mesh.indices[i];
    }
qDebug() << "POST TRILIST";
    std::vector<float> vertexList;
    vertexList.resize(aWidth * aHeight * 4, 0);
    for (uint i = 0; i < mesh.vertices.size(); i++) {
        vertexList[i * 4 + 0] = mesh.vertices[i].x;
        vertexList[i * 4 + 1] = mesh.vertices[i].y;
        vertexList[i * 4 + 2] = mesh.vertices[i].z;
        vertexList[i * 4 + 3] = 1;
    }
qDebug() << "POST VERTEXLIST";
    std::vector<float> normalList;
    normalList.resize(aWidth * aHeight * 4, 0);
    for (uint i = 0; i < mesh.normals.size(); i++) {
        normalList[i * 4 + 0] = mesh.normals[i].x;
        normalList[i * 4 + 1] = mesh.normals[i].y;
        normalList[i * 4 + 2] = mesh.normals[i].z;
        normalList[i * 4 + 3] = 0;
    }
qDebug() << "POST NORMALLIST";
    GLuint triListMap = TextureLoader::createArray(aWidth, aHeight, GL_R32F, GL_RED, GL_FLOAT, &triList[0]);
    GLuint vertexMap = TextureLoader::createArray(aWidth, aHeight, GL_RGBA32F, GL_RGBA, GL_FLOAT, &vertexList[0]);
    GLuint normalMap = TextureLoader::createArray(aWidth, aHeight, GL_RGBA32F, GL_RGBA, GL_FLOAT, &normalList[0]);
qDebug() << "POST ARRAYS";
    float tileSize = 1.0f / TILES;

    for (unsigned int x = 0; x < TILES; x++) {
        for (unsigned int y = 0; y < TILES; y++) {
            //qDebug() << x << " " << y;
            glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
            glDepthMask(GL_FALSE);
            glStencilFunc(GL_NEVER, 1, 0xFF);
            glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);
            glStencilMask(0xFF);
            glClear(GL_STENCIL_BUFFER_BIT);

            tilesShader->bind();
            Matrix4f modelMatrix;
            modelMatrix.translate(Vector3f(-1+tileSize + x*tileSize*2, -1+tileSize + y*tileSize*2, 0));
            modelMatrix.scale(Vector3f(tileSize, tileSize, 1));
            tilesShader->uniformMatrix4f("modelMatrix", modelMatrix);
qDebug() << "X: " << -1+x*tileSize*2 << " Y: " << -1+y*tileSize*2;
            glBindVertexArray(quad->meshes[0].handle);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad->meshes[0].faceVBO);
            glDrawElements(GL_TRIANGLES, quad->meshes[0].indices.size(), GL_UNSIGNED_INT, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
            tilesShader->unbind();

            glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
            glDepthMask(GL_TRUE);
            glStencilMask(0x00);
            glStencilFunc(GL_EQUAL, 1, 0xFF);

            normalShader->bind();
            normalShader->uniform1i("arrayWidth", aWidth);
            normalShader->uniform1i("arrayHeight", aHeight);

            // Upload triangle lists
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, triListMap);
            normalShader->uniform1i("triList", 0);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, vertexMap);
            normalShader->uniform1i("vertexList", 1);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, normalMap);
            normalShader->uniform1i("normalList", 2);

            glBindVertexArray(cage->handle);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cage->faceVBO);
            glDrawElements(GL_TRIANGLES, cage->indices.size(), GL_UNSIGNED_INT, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            glBindVertexArray(0);

            glFinish();
        }
    }

    glDisable(GL_STENCIL_TEST);
qDebug() << "SIZE UCHAR: " << sizeof(unsigned char);
    unsigned char *pixels = new unsigned char[width * height * 4];
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    Image image(width, height);
    image.setData(pixels);
    TextureLoader::storeTexture("res/OutputNRM.bmp", image);

    bakedNormal = bakeBuffer->getTexture();
    bakeBuffer->disable();

    std::clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    qDebug() << "Rendering took: " << elapsed_secs;
    //qDebug() << "Rendering took" << timer.waitForResult() << "microseconds";
}
