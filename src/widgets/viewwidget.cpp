#include "viewwidget.h"

#include "../modelloader.h"
#include "../cage.h"

#include <QTimer>
#include <QDebug>

const int UPDATE_RATE = 10;

ViewWidget::ViewWidget(QWidget* parent) :
    QOpenGLWidget(parent)
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(tick()));
    timer->start(UPDATE_RATE);
}

ViewWidget::~ViewWidget() {
//    timer->stop();
    qDebug() << "Freeing resources";
    delete modelRenderer;
    delete normalMapRenderer;
    qDebug() << "Ready to close...";
}

void ViewWidget::setLowPoly(QString fileName) {
    makeCurrent();
    scene.lowPoly = ModelLoader::loadModel(fileName.toStdString().c_str(), true);
    scene.debugNormals = ModelLoader::createDebugNormals(scene.lowPoly->meshes[0], 0.1f);

    scene.cage = Cage::generateCage(scene.lowPoly->meshes[0], 0.05f);
}

void ViewWidget::setHighPoly(QString fileName) {
    makeCurrent();
    scene.highPoly = ModelLoader::loadModel(fileName.toStdString().c_str(), true);
}

void ViewWidget::setResolution(const unsigned int res) {
    qDebug() << "Setting texture resolution to " << res << "x" << res;
    scene.resolution = res;
}

void ViewWidget::bake(int map) {
    makeCurrent();
    normalMapRenderer->renderNormal(scene);
}

void ViewWidget::save(QString fileName, int map) {
    makeCurrent();
    normalMapRenderer->save(fileName, scene.normalMap);
}

void ViewWidget::tick() {
    update();
}

void ViewWidget::initializeGL() {
    modelRenderer = new ModelRenderer();
    normalMapRenderer = new NormalMapRenderer();

    modelRenderer->init();
    normalMapRenderer->init();

    scene.quad = ModelLoader::loadModel("res/Quad.obj", false);
}

void ViewWidget::resizeGL(int w, int h) {
    modelRenderer->resize(w, h);
    normalMapRenderer->resize(w, h);
}

void ViewWidget::paintGL() {
    modelRenderer->render(scene);
    //normalMapRenderer->render(scene);
}
