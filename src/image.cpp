#include "image.h"

#include "Color.h"
#include <cstdlib>

Image::Image(int width, int height) {
    this->width = width;
    this->height = height;
    this->data = (unsigned char*) calloc(width * height * 3, sizeof(unsigned char));
}

int Image::getWidth() {
    return width;
}

int Image::getHeight() {
    return height;
}

unsigned char* Image::getData() {
    return data;
}

void Image::setData(unsigned char* data) {
    this->data = data;
}

Color Image::getColor(int x, int y) {
    unsigned char r = data[y * 3 * width + x * 3 + 0];
    unsigned char g = data[y * 3 * width + x * 3 + 1];
    unsigned char b = data[y * 3 * width + x * 3 + 2];

    return Color(r, g, b);
}

void Image::setColor(int x, int y, Color c) {
    data[y * 3 * width + x * 3 + 0] = c.r;
    data[y * 3 * width + x * 3 + 1] = c.g;
    data[y * 3 * width + x * 3 + 2] = c.b;
}