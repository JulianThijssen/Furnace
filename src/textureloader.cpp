#include "textureloader.h"

#include <SOIL.h>

#include "Image.h"

GLuint TextureLoader::loadTexture(const char* path) {
    GLuint texture = SOIL_load_OGL_texture
        (
        path.str().c_str(),
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
        );

    return texture;
}

GLuint TextureLoader::loadTextureTrue(const char* path) {
    GLuint texture = SOIL_load_OGL_texture
        (
        path.str().c_str(),
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y
        );

    return texture;
}

void TextureLoader::storeTexture(const char* path, Image image) {
    SOIL_save_image(path, SOIL_SAVE_TYPE_BMP, 256, 256, 3, image.getData());
}

GLuint TextureLoader::createArray(GLsizei width, GLsizei height, GLint internalFormat, GLenum format, GLenum type, const void* pixels) {
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, pixels);
    return texture;
}
