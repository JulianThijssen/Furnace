#include "textureloader.h"

#include <QOpenGLFunctions>
//#include <SOIL.h>

#include "Image.h"
#include <QImage>
#include <QDebug>

GLuint TextureLoader::loadTexture(const char* path) {
//    GLuint texture = SOIL_load_OGL_texture
//        (
//        path,
//        SOIL_LOAD_AUTO,
//        SOIL_CREATE_NEW_ID,
//        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
//        );

    return 0;//return texture;
}

QOpenGLTexture* TextureLoader::loadTextureTrue(const char* path) {
    QImage image = QImage(QString(path));
    qDebug() << image.width();
    QOpenGLTexture* tex = new QOpenGLTexture(QImage(QString(path)).mirrored());

    return tex;
}

void TextureLoader::storeTexture(const char* path, Image* image) {
    //SOIL_save_image(path, SOIL_SAVE_TYPE_BMP, 256, 256, 3, image.getData());
    QImage qimage(image->getWidth(), image->getHeight(), QImage::Format_RGBA8888);
    unsigned char* pixels = image->getData();

    for (unsigned int i = 0; i < image->getHeight(); i++) {
        memcpy(qimage.scanLine(i), pixels + i*image->getWidth()*4, image->getWidth()*4);
    }

    qimage = qimage.mirrored();

    qimage.save(path);
}

GLuint TextureLoader::createArray(GLsizei width, GLsizei height, GLint internalFormat, GLenum format, GLenum type, const void* pixels) {
    QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
    GLuint texture;
    f->glGenTextures(1, &texture);
    f->glBindTexture(GL_TEXTURE_2D, texture);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    f->glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, pixels);
    return texture;
}
