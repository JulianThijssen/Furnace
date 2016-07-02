#include "math.h"

#include <cmath>

const float Math::PI = 3.14159265359f;

float Math::toDegrees(const float radians) {
    return radians * (180 / PI);
}

float Math::toRadians(const float degrees) {
    return degrees * (PI / 180);
}

float Math::clamp(const float f, float low, float high) {
    if (f < low) {
        return low;
    }
    if (f > high) {
        return high;
    }
    return f;
}

int Math::clamp(const int i, int low, int high) {
    if (i < low) {
        return low;
    }
    if (i > high) {
        return high;
    }
    return i;
}

int Math::round(const float f) {
    if ((int) f == (int) f + 0.5) {
        return (int) f;
    }
    else {
        return (int) f + 1;
    }
}

int Math::floor(const float f) {
    return (int) f;
}

int Math::ceil(const float f) {
    return (int) f + 1;
}

unsigned int Math::ceilToPow2(const float f) {
    unsigned int v = (unsigned int) f;
    unsigned int r = 0;

    while (v >>= 1) {
        r++;
    }

    return 2 << r;
}

float Math::sqrt(const float f) {
    return sqrtf(f);
}

float Math::min(const float f1, const float f2, const float f3) {
    if (f1 <= f2 && f1 <= f3) {
        return f1;
    }
    if (f2 <= f1 && f2 <= f3) {
        return f2;
    }
    if (f3 <= f1 && f3 <= f2) {
        return f3;
    }
}

float Math::max(const float f1, const float f2, const float f3) {
    if (f1 >= f2 && f1 >= f3) {
        return f1;
    }
    if (f2 >= f1 && f2 >= f3) {
        return f2;
    }
    if (f3 >= f1 && f3 >= f2) {
        return f3;
    }
}
