//
// Created by anis on 13/03/2026.
//

#include "Image.hh"

#include <fstream>
#include <iostream>

Image::Image(int width, int height, const std::vector<color::RGB>& pixels):
width(width),
height(height),
pixels(pixels) {

}
Image::Image(int width, int height) : width(width), height(height), pixels(std::vector<color::RGB>(width*height, color::RGB{0,0,0})) {}

void Image::save(const std::string &filename) {
    std::ofstream file(filename);
    file << "P3\n" << width << " " << height << "\n255\n";
    int x = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            color::RGB pixel = pixels[x];
            file << " " << static_cast<int>(pixel.get_r()) << " " << static_cast<int>(pixel.get_g()) << " " << static_cast<int>(pixel.get_b()) << " ";
            x+=1;
        }
        file << "\n";
    }
}
