//
// Created by anis on 13/03/2026.
//

#ifndef TP1_IMAGE_HH
#define TP1_IMAGE_HH
#include <string>
#include <vector>

#include "color/RGB.hh"


class Image {
public:
    int get_width() const {
        return width;
    }

    int get_height() const {
        return height;
    }

    std::vector<color::RGB>& get_pixels() {
        return pixels;
    }
    Image(int width, int height, const std::vector<color::RGB>& pixels);
    Image(int width, int height);

    void save(const std::string& filename);

private:
    int width;
    int height;
    std::vector<color::RGB> pixels;
};


#endif //TP1_IMAGE_HH