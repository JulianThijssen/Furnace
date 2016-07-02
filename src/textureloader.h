#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H

class Image;
class GLuint;
class GLenum;

class TextureLoader {
public:
    static GLuint loadTexture(const char* path);
    static GLuint loadTextureTrue(const char* path);
    static void storeTexture(const char* path, Image image);
    static GLuint createArray(int width, int height, int internalFormat, GLenum format, GLenum type, const void* pixels);
private:

};

#endif // TEXTURELOADER_H
