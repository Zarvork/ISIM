//
// Created by anis on 15/03/2026.
//

#include "RGB.hh"

namespace color {
    RGB::RGB(const uint8_t r, const uint8_t g, const uint8_t b) : r(r),g(g),b(b) {};
    std::ostream & operator<<(std::ostream &os, const RGB &obj)
    {
        return os << "RGB(" << obj.r << "," << obj.g << "," << obj.b << ")";
    }
}
