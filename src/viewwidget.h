#ifndef VIEWWIDGET_H
#define VIEWWIDGET_H

#include <QObject>
#include <QWidget>
#include <QOpenGLWidget>

#include <QOpenGLFunctions_3_2_Core>
#include <QString>

#include "model.h"
#include "shader.h"
#include "matrix4f.h"

class ViewWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_2_Core
{
    Q_OBJECT
public:
    ViewWidget(QWidget* parent);
    void setLowPoly(QString fileName);
    void setHighPoly(QString fileName);

public slots:
    void tick();

protected:
    void initializeGL();

    void resizeGL(int w, int h);

    void paintGL();
private:
    Shader* diffuseShader;

    Model* lowPoly;
    Model* highPoly;
//    Matrix4f modelMatrix;
};

#endif // VIEWWIDGET_H
