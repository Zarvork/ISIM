//
// Created by anis on 15/03/2026.
//

#include "Point_Light.hh"

namespace light{
    Point_Light::Point_Light(float intensity, const geometry::Point4 &center)  : intensity(intensity),
    center(center) {};

    float Point_Light::get_intensity() {
        return intensity;
    }

    const geometry::Point4& Point_Light::get_center() {
        return center;
    }
}
