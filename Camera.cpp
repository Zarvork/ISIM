//
// Created by anis on 15/03/2026.
//

#include "Camera.hh"

// Angles are given in degree in the constructor but are stored in radian (conversion is done in constructor)
Camera::Camera(const geometry::Point4 &center, const geometry::Point4 &p, const geometry::Vector4 &up, float alpha, float beta, float z_min)
: center(center),
          p(p),
          up(up),
          alpha(alpha * (std::numbers::pi / 180.0f)),
          beta(beta * (std::numbers::pi / 180.0f)),
          z_min(z_min) {}