//
// Created by anis on 13/03/2026.
//

#include "Point4.hh"

namespace geometry{
    std::ostream & operator<<(std::ostream &os, const Point4 &obj){
        return os
               << "Point4(" << obj.x << "," << obj.y << "," << obj.z << "," << obj.w << ")\n";
    }

    void Point4::homogenize() {
        x = x/w;
        y = y/w;
        z = z/w;
        w = 1;
    }

    Point4::Point4(const float x, const float y, const float z, const float w) :x(x),y(y),z(z),w(w) {
        // Homogeneous coordinates
        homogenize();
    }


    Point4 Point4::operator+(const Vector4& v) const {
        return Point4{x + v.get_x(), y + v.get_y(), z + v.get_z(), w};
    }

    Vector4 Point4::operator-(const Point4& other) const {
        return Vector4{x - other.x, y - other.y, z - other.z};
    }
}


