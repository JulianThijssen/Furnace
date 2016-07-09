#ifndef MATH_H
#define MATH_H


class Math {
public:
    static const float PI;

    static float toDegrees(const float radians);
    static float toRadians(const float degrees);
    static float clamp(const float f, float low, float high);
    static int clamp(const int i, int low, int high);
    static int round(const float f);
    static int floor(const float f);
    static int ceil(const float f);
    static unsigned int ceilToPow2(const float f);
    static float sqrt(const float f);
    static float min(const float f1, const float f2, const float f3);
    static float max(const float f1, const float f2, const float f3);
};

#endif // MATH_H
