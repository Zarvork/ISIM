//
// Created by anis on 13/03/2026.
//

#ifndef TP1_POINT4_HH
#define TP1_POINT4_HH
#include <ostream>

#include "Vector4.hh"

namespace geometry {
    class Point4 {
    public:
        // Getters
        float get_x() const {
            return x;
        }

        float get_y() const {
            return y;
        }

        float get_z() const {
            return z;
        }

        float get_w() const {
            return w;
        }
        // Setters
        void set_x(const float new_x) {
            x = new_x;
        }

        void set_y(const float new_y) {
            y = new_y;
        }

        void set_z(const float new_z) {
            z = new_z;
        }

        void set_w(const float new_w) {
            w = new_w;
            homogenize();
        }

        Point4(float x, float y, float z, float w);
        Point4 operator+(const Vector4& v) const;
        Vector4 operator-(const Point4& other) const;

        void homogenize();

        friend std::ostream & operator<<(std::ostream &os, const Point4 &obj);

    protected:
        float x;
        float y;
        float z;
        float w;
    };
}



#endif //TP1_POINT4_HH