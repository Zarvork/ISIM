//
// Created by anis on 13/03/2026.
//

#ifndef TP1_VECTOR4_HH
#define TP1_VECTOR4_HH

#include <ostream>

namespace geometry {
    class Vector4 {
    public:
        float get_x() const {
            return x;
        }

        void set_x(const float new_x) {
            x = new_x;
        }

        float get_y() const {
            return y;
        }

        void set_y(const float new_y) {
            y = new_y;
        }

        float get_z() const {
            return z;
        }

        void set_z(const float new_z) {
            z = new_z;
        }

        Vector4 operator+(const Vector4 & v) const;
        Vector4();
        Vector4(float x, float y, float z);
        Vector4 operator*(const float &i) const;
        float dotProduct(const Vector4 &v) const;
        void normalize();

        Vector4 cross(const Vector4 &v) const;

        Vector4 operator-(const Vector4 &v) const;
        float norm() const;
        friend std::ostream & operator<<(std::ostream &os, const Vector4 &obj);
    private:
        float x, y, z;
    };
}



#endif //TP1_VECTOR4_HH