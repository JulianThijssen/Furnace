#ifndef VIEWWIDGET_H
#define VIEWWIDGET_H

#include <QObject>
#include <QWidget>
#include <QOpenGLWidget>

#include <QOpenGLFunctions_3_2_Core>
#include <QOpenGLTexture>
#include <QString>
#include <QTimer>

#include "../framebuffer.h"
#include "../model.h"
#include "../shader.h"
#include "../image.h"
#include "../util/matrix4f.h"

class ViewWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_2_Core
{
    Q_OBJECT
public:
    ViewWidget(QWidget* parent);
    ~ViewWidget();

    void setLowPoly(QString fileName);
    void setHighPoly(QString fileName);
    void setResolution(const unsigned int resolution);
    void save(QString fileName);

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
    Shader* colorShader;

    Mesh* cage;
    Model* lowPoly;
    Model* highPoly;
    Model* quad;

    QOpenGLTexture* texture;
    GLuint debugNormals = 0;
    GLuint bakedNormal = 0;

    unsigned int resolution;
    Image* normalMap;
};

#endif // VIEWWIDGET_H
