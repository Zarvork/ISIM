//
// Created by anis on 18/03/2026.
//

#include "RGB_f.hh"

namespace color {
    RGB_f::RGB_f(const float r, const float g, const float b) : r(r),g(g),b(b) {};
    std::ostream & operator<<(std::ostream &os, const RGB_f &obj)
    {
        return os << "RGB_f(" << obj.r << "," << obj.g << "," << obj.b << ")";
    }
}