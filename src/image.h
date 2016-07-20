#ifndef IMAGE_H
#define IMAGE_H

class Color;

class Image {
public:
    Image(int width, int height);
    unsigned int getWidth();
    unsigned int getHeight();
    unsigned char* getData();
    void Image::setData(unsigned char* pixels);

    Color getColor(int x, int y);
    void setColor(int x, int y, Color c);
private:
    unsigned int width;
    unsigned int height;
    unsigned char* data;
};


#endif // IMAGE_H
