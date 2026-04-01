//
// Created by anis on 13/03/2026.
//

#include "Sphere.hh"

#include <cmath>

namespace object {
    Sphere::Sphere(texture::Texture_Material &material, const geometry::Point4 &center, float radius)
        : Object(material), center(center), radius(radius){};

    std::optional<geometry::Point4> Sphere::does_ray_intersect(const geometry::Point4& p, const geometry::Vector4& v) const {
        float t0;
        float t1;
        geometry::Vector4 L = center - p;
        float tca = L.dotProduct(v);
        if (tca < 0) return std::nullopt;
        float d2 = L.dotProduct(L) - tca * tca;
        if (d2 > radius * radius) return std::nullopt;
        float thc = std::sqrt(radius * radius - d2);
        t0 = tca - thc;
        t1 = tca + thc;

        if (t0 < 0) {
            t0 = t1;
            if (t0 < 0) return std::nullopt;
        }
        auto intersection_point = p + v * t0;
        return std::optional<geometry::Point4>{intersection_point};
    }
    geometry::Vector4 Sphere::get_normal_vector(const geometry::Point4& p) const {
        geometry::Vector4 result = p - center;
        result.normalize();
        return result;
    }
    texture::SurfaceProperties Sphere::get_texture(const geometry::Point4& p) const{
        return material.get_properties(p);
    }
}
