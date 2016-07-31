#ifndef VIEWWIDGET_H
#define VIEWWIDGET_H

#include "../scene.h"
#include "../modelrenderer.h"
#include "../normalmaprenderer.h"

#include <QObject>
#include <QWidget>
#include <QOpenGLWidget>
#include <QString>
#include <QTimer>

class ViewWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    ViewWidget(QWidget* parent);
    ~ViewWidget();

    void setView(const unsigned int view);
    void setLowPoly(QString fileName);
    void setHighPoly(QString fileName);
    void setResolution(const unsigned int res);

    void bake(int map);
    void save(QString fileName, int map);

    ModelRenderer* getModelRenderer() {
        return modelRenderer;
    }

public slots:
    void tick();

protected:
    void initializeGL();

    void resizeGL(int w, int h);

    void paintGL();
private:
    Scene scene;
    ModelRenderer* modelRenderer;
    NormalMapRenderer* normalMapRenderer;

    QTimer* timer;

    unsigned int view;
};

#endif // VIEWWIDGET_H
