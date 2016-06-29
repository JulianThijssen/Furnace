#ifndef VECTOR2F_H
#define VECTOR2F_H

#include <string>

class Vector2f
{
public:
    float x, y;

    static const Vector2f ZERO;
    static const Vector2f UP;

    /* Core */
    Vector2f();
    Vector2f(float x, float y);
    void set(float x, float y);
    void set(const Vector2f& v);
    Vector2f* add(const Vector2f& v);
    Vector2f* sub(const Vector2f& v);
    Vector2f* scale(const float scale);
    Vector2f* normalise();

    /* Operator overloads */
    bool operator==(const Vector2f& v) const;
    bool operator!=(const Vector2f& v) const;
    Vector2f* operator+=(const Vector2f& v);
    Vector2f* operator-=(const Vector2f& v);
    Vector2f* operator*=(const Vector2f& v);
    Vector2f* operator/=(const Vector2f& v);
    Vector2f operator+(const Vector2f& v) const;
    Vector2f operator-(const Vector2f& v) const;
    Vector2f operator-() const;
    Vector2f operator*(const float scale) const;
    Vector2f operator/(const float divisor) const;
};

/* Utility functions */
float length(const Vector2f& v);
float dot(const Vector2f& v1, const Vector2f& v2);
Vector2f negate(const Vector2f& v);
Vector2f normalise(const Vector2f& v);
std::string str(const Vector2f& v);

#endif // VECTOR2F_H
