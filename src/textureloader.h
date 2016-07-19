#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H

#include <QOpenGLFunctions>
#include <QOpenGLTexture>

class Image;

class TextureLoader {
public:
    static GLuint loadTexture(const char* path);
    static QOpenGLTexture* loadTextureTrue(const char* path);
    static void storeTexture(const char* path, Image* image);
    static GLuint createArray(int width, int height, int internalFormat, GLenum format, GLenum type, const void* pixels);
private:

};

#endif // TEXTURELOADER_H
