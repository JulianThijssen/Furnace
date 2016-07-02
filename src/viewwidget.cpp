#include "viewwidget.h"

#include "shaderloader.h"
#include "modelloader.h"
#include "model.h"
#include "mesh.h"

#include <QTimer>
#include <QtMath>
#include <QDebug>

const int UPDATE_RATE = 10;

ViewWidget::ViewWidget(QWidget* parent) :
    QOpenGLWidget(parent),
    lowPoly(nullptr)
{
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(tick()));
    timer->start(UPDATE_RATE);
}

void ViewWidget::setLowPoly(QString fileName) {
    makeCurrent();
    lowPoly = ModelLoader::loadModel(fileName.toStdString().c_str(), true);
}

void ViewWidget::setHighPoly(QString fileName) {
    makeCurrent();
    highPoly = ModelLoader::loadModel(fileName.toStdString().c_str(), true);
}

void ViewWidget::tick() {
    qDebug() << "UPDATING";
    update();
}

void ViewWidget::initializeGL() {
    initializeOpenGLFunctions();

    diffuseShader = ShaderLoader::loadShaders("res/diffuse.vert", "res/diffuse.frag");
    glClearColor(0, 0, 0, 1);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    qDebug() << lowPoly;
}

void ViewWidget::resizeGL(int w, int h) {

}

void ViewWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    qDebug() << lowPoly;
    if (lowPoly) {
        diffuseShader->bind();

        Matrix4f projMatrix;
        float fovyr = qDegreesToRadians(90.f);
        projMatrix[0] = (float)(1 / tan(fovyr / 2)) / 1;
        projMatrix[5] = (float)(1 / tan(fovyr / 2));
        projMatrix[10] = (0.1f + 100) / (0.1f - 100);
        projMatrix[11] = -1;
        projMatrix[14] = (2 * 0.1f * 100) / (0.1f - 100);
        projMatrix[15] = -0;
        diffuseShader->uniformMatrix4f("projMatrix", projMatrix);

        Matrix4f modelMatrix;
        modelMatrix.setIdentity();
        modelMatrix.translate(Vector3f(0, 0, -10.f));
        modelMatrix.rotate(45, 0, 1, 0);
        //modelMatrix.scale(Vector3f(0.5f, 0.5f, 0.5f));
        diffuseShader->uniformMatrix4f("modelMatrix", modelMatrix);

        Mesh mesh = lowPoly->meshes[0];
        glBindVertexArray(mesh.handle);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.faceVBO);
        glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        qDebug() << "PAINTING";
    }
}
