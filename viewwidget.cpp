#include "viewwidget.h"

void ViewWidget::initializeGL() {
    initializeOpenGLFunctions();

    glClearColor(0, 0, 0, 1);
}

void ViewWidget::resizeGL(int w, int h) {

}

void ViewWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);
}
