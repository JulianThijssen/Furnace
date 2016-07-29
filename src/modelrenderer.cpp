#include "modelrenderer.h"

#include "shaderloader.h"

#include <QtMath>
#include <QDebug>

const int UPDATE_RATE = 10;

ModelRenderer::ModelRenderer() :
    diffuseShader(nullptr),
    colorShader(nullptr),
    cageVisible(false)
{

}

ModelRenderer::~ModelRenderer() {
    delete diffuseShader;
    delete colorShader;
    qDebug() << "ModelRenderer destroyed!";
}

void ModelRenderer::showCage(bool show) {
    cageVisible = show;
    qDebug() << "Showing cage:" << show;
}

void ModelRenderer::init() {
    initializeOpenGLFunctions();

    diffuseShader = ShaderLoader::loadShaders("res/diffuse.vert", "res/diffuse.frag");
    colorShader = ShaderLoader::loadShaders("res/color.vert", "res/color.frag");

    glClearColor(0, 0, 0, 1);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

void ModelRenderer::resize(int w, int h) {
    qDebug() << "RESIZING";

    Matrix4f projMatrix;
    float fovyr = qDegreesToRadians(90.f);
    projMatrix[0] = (float)(1 / tan(fovyr / 2)) / ((float) w / h);
    projMatrix[5] = (float)(1 / tan(fovyr / 2));
    projMatrix[10] = (0.1f + 100) / (0.1f - 100);
    projMatrix[11] = -1;
    projMatrix[14] = (2 * 0.1f * 100) / (0.1f - 100);
    projMatrix[15] = -0;

    diffuseShader->bind();
    diffuseShader->uniformMatrix4f("projMatrix", projMatrix);
    diffuseShader->unbind();
    colorShader->bind();
    colorShader->uniformMatrix4f("projMatrix", projMatrix);
    colorShader->unbind();
}

float t = 0;
void ModelRenderer::render(const Scene& scene) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (scene.lowPoly) {
        t += 0.2f;
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        diffuseShader->bind();

        if (scene.bakedNormal) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, scene.bakedNormal);
            diffuseShader->uniform1i("albedo", 0);
            diffuseShader->uniform1i("hasAlbedo", 1);
        } else {
            diffuseShader->uniform1i("hasAlbedo", 0);
        }

        Matrix4f modelMatrix;
        modelMatrix.setIdentity();
        modelMatrix.translate(Vector3f(0, 0, -1.5f));
        modelMatrix.rotate(t, 0, 1, 0);
        //modelMatrix.scale(Vector3f(0.5f, 0.5f, 0.5f));
        diffuseShader->uniformMatrix4f("modelMatrix", modelMatrix);

        Mesh mesh = scene.lowPoly->meshes[0];
        mesh.bind();
        mesh.draw();
        mesh.unbind();

        colorShader->bind();
        colorShader->uniformMatrix4f("modelMatrix", modelMatrix);

        // Render cage
        if (cageVisible) {
            colorShader->uniform3f("color", 1, 0.5f, 0);

            glDisable(GL_CULL_FACE);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            scene.cage->bind();
            scene.cage->draw();
            scene.cage->unbind();
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glEnable(GL_CULL_FACE);
        }

        // Render normals
        colorShader->uniform3f("color", 0, 1, 1);
        glBindVertexArray(scene.debugNormals);
        glDrawArrays(GL_LINES, 0, (GLsizei) mesh.vertices.size() * 2);
        glBindVertexArray(0);
        colorShader->unbind();
    }
}
