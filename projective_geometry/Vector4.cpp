//
// Created by anis on 13/03/2026.
//

#include "Vector4.hh"

#include <cmath>

namespace geometry {
    Vector4::Vector4(const float x, const float y, const float z) : x(x), y(y), z(z) {
    }

    Vector4::Vector4(): x(0), y(0), z(0) {

    }

    Vector4 Vector4::operator*(const float &i) const {
        return Vector4{i * x, i * y, i * z};
    }

    Vector4 Vector4::operator-(const Vector4 &v) const {
        return Vector4{x - v.x, y - v.y, z - v.z};
    }

    std::ostream & operator<<(std::ostream &os, const Vector4 &obj) {
        return os << "Vector4(" << obj.x << "," << obj.y << "," << obj.z << ")\n";;
    }

    float Vector4::norm() const {
        return std::sqrt(x*x + y*y + z*z);
    }

    float Vector4::dotProduct(const Vector4 &v) const {
        return x * v.x + y * v.y + z * v.z;
    }

    void Vector4::normalize() {
        float norm_value = norm();
        x /= norm_value;
        y /= norm_value;
        z /= norm_value;
    }

    Vector4 Vector4::cross(const Vector4 &v) const {
        Vector4 result{ y*v.z - z*v.y,z*v.x - x*v.z,x*v.y - y*v.x};
        return result;
    }

    Vector4 Vector4::operator+(const Vector4 & v) const {
        return Vector4{x + v.x, y + v.y, z + v.z};
    }


}


