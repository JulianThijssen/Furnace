#ifndef VIEWWIDGET_H
#define VIEWWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_2_Core>

class ViewWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_2_Core
{
public:
    ViewWidget(QWidget* parent) : QOpenGLWidget(parent) { }
protected:
    void initializeGL();

    void resizeGL(int w, int h);

    void paintGL();
};

#endif // VIEWWIDGET_H
