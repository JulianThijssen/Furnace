#ifndef COLOR_H
#define COLOR_H

class Vector3f;

class Color {
public:
    unsigned char r;
    unsigned char g;
    unsigned char b;

    Color(unsigned char r, unsigned char g, unsigned char b);
    bool isBlack();
    static Vector3f toVector(Color color);
    static Color fromVector(Vector3f vector);
private:

};

#endif // COLOR_H
