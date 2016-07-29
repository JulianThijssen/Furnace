#ifndef MODELRENDERER_H
#define MODELRENDERER_H

#include <QOpenGLFunctions_3_2_Core>
#include <QOpenGLTexture>
#include <QString>

#include "framebuffer.h"
#include "model.h"
#include "shader.h"
#include "image.h"
#include "scene.h"

#include "util/matrix4f.h"

class ModelRenderer : protected QOpenGLFunctions_3_2_Core
{
public:
    ModelRenderer();
    ~ModelRenderer();

    void showCage(bool show);

    void init();
    void resize(int w, int h);
    void render(const Scene& scene);
private:
    Shader* diffuseShader;
    Shader* colorShader;

    bool cageVisible;
};

#endif // MODELRENDERER_H
