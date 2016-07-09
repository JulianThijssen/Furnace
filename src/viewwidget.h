#ifndef VIEWWIDGET_H
#define VIEWWIDGET_H

#include <QObject>
#include <QWidget>
#include <QOpenGLWidget>

#include <QOpenGLFunctions_3_2_Core>
#include <QOpenGLTexture>
#include <QString>
#include <QTimer>

#include "framebuffer.h"
#include "model.h"
#include "shader.h"
#include "util/matrix4f.h"

class ViewWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_2_Core
{
    Q_OBJECT
public:
    ViewWidget(QWidget* parent);
    ~ViewWidget();

    void setLowPoly(QString fileName);
    void setHighPoly(QString fileName);

    void renderNormal(const int width, const int height);

public slots:
    void tick();

protected:
    void initializeGL();

    void resizeGL(int w, int h);

    void paintGL();
private:
    QTimer* timer;

    Framebuffer* bakeBuffer;

    Shader* diffuseShader;
    Shader* normalShader;
    Shader* tilesShader;

    Mesh* cage;
    Model* lowPoly;
    Model* highPoly;
    Model* quad;

    QOpenGLTexture* texture;
    GLuint bakedNormal = 0;
};

#endif // VIEWWIDGET_H
