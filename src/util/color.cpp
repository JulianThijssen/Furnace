#include "color.h"

#include "Vector3f.h"

Color::Color(unsigned char r, unsigned char g, unsigned char b) {
    this->r = r;
    this->g = g;
    this->b = b;
}

bool Color::isBlack() {
    return r == 0 && g == 0 && b == 0;
}

Vector3f Color::toVector(Color color) {
    float x = (color.r / 128.0f) - 1;
    float y = (color.g / 128.0f) - 1;
    float z = (color.b / 128.0f) - 1;
    return Vector3f(x, y, z);
}

Color Color::fromVector(Vector3f vector) {
    unsigned char r = (vector.x + 1) * 128;
    unsigned char g = (vector.y + 1) * 128;
    unsigned char b = (vector.z + 1) * 128;
    return Color(r, g, b);
}
