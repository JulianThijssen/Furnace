#ifndef NORMALMAPRENDERER_H
#define NORMALMAPRENDERER_H

#include "framebuffer.h"
#include "model.h"
#include "shader.h"
#include "image.h"
#include "scene.h"
#include "util/matrix4f.h"

#include <QOpenGLFunctions_3_2_Core>
#include <QOpenGLTexture>
#include <QString>

class NormalMapRenderer : protected QOpenGLFunctions_3_2_Core
{
public:
    NormalMapRenderer();
    ~NormalMapRenderer();

    void init();
    void resize(int w, int h);
    void render(const Scene& scene);
    void renderNormal(Scene& scene);

    void setResolution(const unsigned int resolution);
    void save(QString fileName, Image* image);
private:
    Framebuffer* bakeBuffer;

    Shader* diffuseShader;
    Shader* normalShader;
    Shader* tilesShader;
};

#endif // NORMALMAPRENDERER_H
