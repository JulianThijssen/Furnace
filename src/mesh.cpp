#include "mesh.h"

#include <QOpenGLFunctions_3_2_Core>

void Mesh::bind() {
    QOpenGLFunctions_3_2_Core *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_2_Core>();

    f->glBindVertexArray(handle);
    f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceVBO);
}

void Mesh::draw() {
    QOpenGLFunctions_3_2_Core *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_2_Core>();

    f->glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void Mesh::unbind() {
    QOpenGLFunctions_3_2_Core *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_2_Core>();

    f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    f->glBindVertexArray(0);
}
