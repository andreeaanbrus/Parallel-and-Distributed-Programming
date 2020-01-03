#include "lib/jpglib.hpp"
#include <string>

class Pixel {
private:
    unsigned char *red;
    unsigned char *green;
    unsigned char *blue;
public:
    Pixel(unsigned char *red, unsigned char *green, unsigned char *blue) :
            red(red), green(green), blue(blue) {}

    void setColors(const unsigned char &red, const unsigned char &green, const unsigned char &blue) {
        *(this->red) = red;
        *(this->green) = green;
        *(this->blue) = blue;
    }

    void setPixel(const Pixel &pixel) {
        *(this->red) = pixel.getRed();
        *(this->green) = pixel.getGreen();
        *(this->blue) = pixel.getBlue();
    }

    unsigned char getRed() const {
        return *red;
    }

    unsigned char getGreen() const {
        return *green;
    }

    unsigned char getBlue() const {
        return *blue;
    }
};

class Picture {
private:
    int height;
    int width;
    int size;
    int bpp;
    unsigned char *rgb_image;
public:
    explicit Picture(const std::string &fileName) : width(), height(), size(), bpp() {
        rgb_image = stbi_load(fileName.c_str(), &width, &height, &bpp, 3);
        size = height * width * bpp;
    }

    ~Picture() {
        stbi_image_free(rgb_image);
    }

    void write(const std::string &fileName) {
        int res = stbi_write_jpg(fileName.c_str(), width, height, bpp, rgb_image, 100);
    }

    Pixel operator[](int i) const {
        return Pixel(&rgb_image[3 * i], &rgb_image[3 * i + 1], &rgb_image[3 * i + 2]);
    }

    Pixel operator()(int x, int y) const {
        int i = (x * width + y) * 3;
        return Pixel(&rgb_image[i], &rgb_image[i + 1], &rgb_image[i + 2]);
    }

    unsigned int getSize() const {
        return (unsigned int) size;
    }

    unsigned int getPixelCount() const {
        return (unsigned int) (size / 3);
    }

    unsigned int getWidth() const {
        return static_cast<unsigned int>(width);
    }

    unsigned int getHeight() const {
        return static_cast<unsigned int>(height);
    }
};

