#include "vector2f.h"

#include <cmath>
#include <sstream>

const Vector2f Vector2f::ZERO = Vector2f(0, 0);
const Vector2f Vector2f::UP = Vector2f(0, 1);

/* Core */
Vector2f::Vector2f() : x(0), y(0) {}

Vector2f::Vector2f(float x, float y) : x(x), y(y) {}

void Vector2f::set(float x, float y) {
    this->x = x;
    this->y = y;
}

void Vector2f::set(const Vector2f& v) {
    x = v.x;
    y = v.y;
}

Vector2f* Vector2f::add(const Vector2f& v) {
    x += v.x;
    y += v.y;
    return this;
}

Vector2f* Vector2f::sub(const Vector2f& v) {
    x -= v.x;
    y -= v.y;
    return this;
}

Vector2f* Vector2f::scale(const float scale) {
    x *= scale;
    y *= scale;
    return this;
}

Vector2f* Vector2f::normalise() {
    float l = length(*this);
    x /= l;
    y /= l;
    return this;
}

/* Operator overloads */
bool Vector2f::operator==(const Vector2f& v) const {
    return x == v.x && y == v.y;
}

bool Vector2f::operator!=(const Vector2f& v) const {
    return x != v.x || y != v.y;
}

Vector2f* Vector2f::operator+=(const Vector2f& v) {
    x += v.x;
    y += v.y;
    return this;
}

Vector2f* Vector2f::operator-=(const Vector2f& v) {
    x -= v.x;
    y -= v.y;
    return this;
}

Vector2f* Vector2f::operator*=(const Vector2f& v) {
    x *= v.x;
    y *= v.y;
    return this;
}

Vector2f* Vector2f::operator/=(const Vector2f& v) {
    x /= v.x;
    y /= v.y;
    return this;
}

Vector2f Vector2f::operator+(const Vector2f& v) const {
    return Vector2f(x + v.x, y + v.y);
}

Vector2f Vector2f::operator-(const Vector2f& v) const {
    return Vector2f(x - v.x, y - v.y);
}

Vector2f Vector2f::operator-() const {
    return Vector2f(-x, -y);
}

Vector2f Vector2f::operator*(const float scale) const {
    return Vector2f(x * scale, y * scale);
}

Vector2f Vector2f::operator/(const float divisor) const {
    return Vector2f(x / divisor, y / divisor);
}

/* Utility functions */
float length(const Vector2f& v) {
    return sqrt(v.x * v.x + v.y * v.y);
}

float dot(const Vector2f& v1, const Vector2f& v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

Vector2f negate(const Vector2f& v) {
    return Vector2f(-v.x, -v.y);
}

Vector2f normalise(const Vector2f& v) {
    float l = length(v);
    return Vector2f(v.x / l, v.y / l);
}

std::string str(const Vector2f& v) {
    std::stringstream ss;
    ss << "(" << v.x << ", " << v.y << ")";
    return ss.str();
}
